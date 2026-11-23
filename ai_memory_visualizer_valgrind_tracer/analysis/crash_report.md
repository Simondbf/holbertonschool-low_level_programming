# Rapport de crash — Tâche 2 (AI Assisted Crash Report)

* **Programme analysé :** `programs/crash_example.c` (binaire `./crash`, sans modification du code source)
* **Environnement :** Linux x86-64 (`sizeof(int) == 4`, `sizeof(void *) == 8`)
* **Validation croisée :** Exécution native + Valgrind Memcheck 3.26.0 (`analysis/memory_maps.md`, `analysis/valgrind_analysis.md`)

---

## 1. Description du crash

L'exécution de `./crash` se termine de manière **100 % déterministe** par un arrêt brutal du processus (`Segmentation fault`, signal `11` / `SIGSEGV`) immédiatement après l'affichage de `requesting n=0`.

| Observation | Exécution native (`./crash`) | Exécution sous Valgrind (`Memcheck`) |
| :--- | :--- | :--- |
| **Sortie avant arrêt** | `crash_example: deterministic NULL dereference (segmentation fault)`<br>`  requesting n=0` | Identique |
| **Instruction fautive** | `nums[0] = 42;` (`crash_example.c:32`) | `Invalid write of size 4` à `main (crash_example.c:32)` (`0x400128A`) |
| **Adresse cible** | `0x0` (`NULL`) | `Address 0x0 is not stack'd, malloc'd or (recently) free'd` |
| **Signal et issue** | `SIGSEGV` (signal 11) — terminaison immédiate | `Access not within mapped region at address 0x0` → `SIGSEGV` |
| **Code non atteint** | Lignes 34 (`printf`), 36 (`free(nums)`), 37 (`return 0`) | Identique |

---

## 2. Analyse de la cause racine (*Root Cause Analysis*)

Le crash n'est ni aléatoire ni dû à un échec d'allocation système : il découle d'une **absence de vérification de valeur de retour `NULL`** dans `main()` après un appel qui garantit ce retour pour `n <= 0`.

### Chaîne causale complète (du code source au signal `SIGSEGV`)

| Étape | Emplacement | Opération effectuée | État mémoire résultant |
| :-: | :--- | :--- | :--- |
| 1 | `crash_example.c:24-25` | `int *nums = NULL; int n = 0;` | Variables locales initialisées sur la pile de `main()` (`nums = 0x0`, `n = 0`). |
| 2 | `crash_example.c:30` | `nums = allocate_numbers(n);` | Appel de `allocate_numbers(0)` : création d'un cadre de pile (`n = 0`, `arr = NULL`, `i = 0`). |
| 3 | `crash_example.c:9-10` | `if (n <= 0) return NULL;` | Condition vraie (`0 <= 0`) : sortie anticipée **avant** `malloc()` (ligne 12). Le cadre est dépilé. |
| 4 | `crash_example.c:30` | Affectation du retour dans `nums` | `nums` (sur la pile de `main()`) vaut toujours `0x0` (`NULL`). |
| 5 | `crash_example.c:32` | `nums[0] = 42;` (`*(nums + 0) = 42;`) | Aucune garde `if (!nums)` : tentative d'écriture de 4 octets (`int`) à l'adresse virtuelle `0x0`. |
| 6 | Matériel / Noyau | Faute de page (*page fault*) sur `0x0` | Adresse hors région mappée → le noyau Linux délivre `SIGSEGV` → arrêt immédiat. |

### Zone mémoire impliquée : pile, tas, ou autre ?

| Zone mémoire | Rôle exact dans ce crash |
| :--- | :--- |
| **Pile (Stack)** | Contient les variables locales `n` et `nums` de `main()`. La lecture de la valeur `0x0` stockée dans `nums` est **valide** : la pile n'est pas corrompue ni saturée. |
| **Tas (Heap)** | **Aucun bloc utilisateur n'est alloué** (`malloc` ligne 12 n'est jamais atteint). Le seul bloc présent sur le tas au moment du crash est le tampon interne `stdout` de `glibc` (1 024 o alloué à la ligne 27 via `printf`/`puts`), totalement étranger au crash. |
| **Adresse cible (`0x0`)** | L'accès invalide ne touche **ni la pile ni le tas** : il cible l'adresse virtuelle `0x0` (page zéro, non mappée). |

---

## 3. Pourquoi l'accès mémoire est invalide

1. **Au niveau du langage C (comportement indéfini) :**
   L'expression `nums[0] = 42;` constitue un **déréférencement de pointeur nul (*NULL pointer dereference*)**, qui est un **comportement indéfini (*Undefined Behavior*)** selon la norme C. Un pointeur nul est garanti ne désigner aucun objet mémoire valide.
2. **Au niveau matériel et système d'exploitation (Linux x86-64) :**
   L'instruction machine tente d'écrire 4 octets à l'adresse virtuelle `0x0`. Sur Linux, les premières pages de l'espace d'adressage virtuel (dont la page `0x0`, protégée par `vm.mmap_min_addr`) sont volontairement laissées **non mappées** dans la table des pages du processus. Lorsque le processeur tente cette écriture, la MMU déclenche une **faute de page matérielle (*page fault*)** ; le noyau constate que l'adresse n'appartient à aucune région virtuelle valide et envoie le signal `SIGSEGV` (signal 11), dont l'action par défaut termine immédiatement le processus.

---

## 4. Évaluation critique des explications et correctifs proposés par l'IA

| Suggestion de l'IA (cause ou correctif) | Verdict | Justification technique (code + exécution) |
| :--- | :---: | :--- |
| *« `allocate_numbers(0)` retourne `NULL` à cause de la garde `if (n <= 0)`, et `main()` déréférence `nums` ligne 32 sans vérifier `if (!nums)`. »* | **Correct** | Directement vérifiable aux lignes 9-10, 30 et 32. Explique intégralement la chaîne causale sans hypothèse externe. |
| *« Ajouter une vérification `if (!nums)` avant `nums[0] = 42;` pour gérer proprement le retour `NULL`. »* | **Correct** | Traite la cause racine structurelle : protège contre `n <= 0` **et** contre un échec réel de `malloc()` (ligne 13). |
| *« Le crash vient d'un échec de `malloc(0)` (manque de mémoire ou retour `NULL` de `malloc`). »* | **Incorrect** | `malloc()` (ligne 12) **n'est jamais exécuté** : la fonction quitte dès les lignes 9-10 (`if (n <= 0) return NULL;`). |
| *« Le crash implique une corruption du tas ou un débordement de pile (*stack overflow*). »* | **Spéculatif / Faux** | Le tas utilisateur est vide (`0` octet alloué par le programme) et la pile n'utilise que quelques dizaines d'octets sur 8 Mo (`8 388 608` o). Le message générique de Valgrind évoquant `--main-stacksize` ne s'applique pas ici. |
| *« Il suffit de remplacer `int n = 0;` par `int n = 5;` dans `main()`. »* | **Incomplet** | Masque le symptôme pour cette entrée précise, mais laisse intacte la faille : si `malloc()` échoue ou si `n <= 0` revient, `nums[0] = 42;` provoquera exactement le même `SIGSEGV`. |

---

## 5. Correctif suggéré *(optionnel — non appliqué au code source)*

Le correctif rigoureux consiste en priorité à **vérifier le pointeur retourné** avant tout déréférencement (et, si l'intention fonctionnelle est d'allouer un tableau, à passer une taille strictement positive `n > 0`) :

```c
nums = allocate_numbers(n);
if (!nums)
{
    fprintf(stderr, "  allocation failed or invalid size (n=%d)\n", n);
    return 1;
}

nums[0] = 42;
```

---

## 6. Résumé

* **Cause racine déterministe :** `main()` appelle `allocate_numbers(0)` (ligne 30), qui retourne `NULL` avant tout `malloc` (`if (n <= 0)`, lignes 9-10), puis écrit à `nums[0] = 42` (ligne 32) sans vérifier `if (!nums)`.
* **Catégorie de bug :** **déréférencement de pointeur nul (*NULL pointer dereference*, comportement indéfini)** ciblant l'adresse virtuelle `0x0`, qui n'appartient ni à la pile ni au tas (page zéro non mappée → faute de page MMU → `SIGSEGV`).
* **Clarté sur les zones mémoire :** `nums` vit sur la pile, le tas utilisateur reste vide (seul le tampon `stdout` `glibc` de 1 024 o existe, sans rapport avec le crash), et l'écriture fautive vise `0x0`.
* **Tri critique des pistes IA :** les diagnostics pointant la garde `if (n <= 0)` et l'absence de `if (!nums)` sont validés ; les hypothèses d'échec de `malloc(0)`, de corruption du tas ou de *stack overflow* sont réfutées, et le simple passage à `n = 5` est insuffisant sans garde défensive.
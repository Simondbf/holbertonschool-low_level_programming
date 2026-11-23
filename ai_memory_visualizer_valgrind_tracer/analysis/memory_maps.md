# Cartographies mémoire — Tâche 0 (AI Memory Visualizer)

* **Plateforme :** Linux x86-64 (`sizeof(int) == 4`, `sizeof(void *) == 8`, `sizeof(Person) == 16`)
* **Cibles :** `stack`, `heap`, `aliasing`, `crash` (compilés depuis `programs/` sans modification du code source)
* **Méthode :** Exécution native sans Valgrind, relevé des adresses réelles et vérification critique des cartographies générées par IA
* **Note de compilation :** `aliasing_example.c` provoque volontairement un *use-after-free* bloqué par `-Werror` sur GCC récent ; compilé avec `-Wno-error=use-after-free` sans altérer le code C

---

## 1. Tableau de synthèse global

| Programme | Pile (Stack) | Tas (Heap utilisateur) | Alias de pointeurs | Comportement observé en exécution native |
| :--- | :--- | :--- | :--- | :--- |
| **`stack_example.c`** | 4 cadres `walk_stack` (`-48` o/appel) + `dump_frame` réutilisé | 0 o | `p_local == &local_int` | Aucun bug — réutilisation exacte des adresses de pile entre `[enter]` et `[exit]` |
| **`heap_example.c`** | `alice`, `bob` (`main`), `p` (`person_free_partial`) | 4 blocs (42 o demandés, 32 o/chunk `glibc`) | `p == alice` dans `person_free_partial` | **Fuite mémoire de 6 o** (`"Alice\0"`, *definitely lost* par perte de propriété) |
| **`aliasing_example.c`** | `n`, `a`, `b` (`main`) | 1 bloc de 20 o (`int[5]`) | `b == a == 0x5f5c9f3c0420` | ***Use-after-free* silencieux** : `b[2]=-738082945` (métadonnées `tcache`), `b[3]=1234` (corruption), aucun crash |
| **`crash_example.c`** | `n`, `nums` (`main`) | 0 o (`malloc` non appelé) | Aucun | Déréférencement de `NULL` (`0x0`) → **`SIGSEGV` immédiat** |

---

## 2. Analyse détaillée par programme

### 2.1 `stack_example.c` — Récursion et cadres de pile (*stack frames*)

La pile croît vers les **adresses basses** par pas exacts de `-0x30` (**-48 octets**) à chaque appel récursif de `walk_stack()`. Chaque appel à `dump_frame()` est dépilé immédiatement avant l'instruction suivante : son espace est réutilisé à l'identique entre `[enter]` et `[exit]` à profondeur égale.

| Profondeur | `&marker` (`walk_stack`) | Écart | `&local_int` (`[enter]` et `[exit]`) | `p_local` (alias local) | `local_buf` |
| :---: | :--- | :---: | :--- | :--- | :--- |
| `depth=0` | `0x7fff383e17c4` (`0`) | — | `0x7fff383e1774` (`100`) | `0x7fff383e1774` | `0x7fff383e1780` (`"A"`) |
| `depth=1` | `0x7fff383e1794` (`10`) | `-48` o | `0x7fff383e1744` (`101`) | `0x7fff383e1744` | `0x7fff383e1750` (`"B"`) |
| `depth=2` | `0x7fff383e1764` (`20`) | `-48` o | `0x7fff383e1714` (`102`) | `0x7fff383e1714` | `0x7fff383e1720` (`"C"`) |
| `depth=3` | `0x7fff383e1734` (`30`) | `-48` o | `0x7fff383e16e4` (`103`) | `0x7fff383e16e4` | `0x7fff383e16f0` (`"D"`) |

```text
PILE au pic ([enter] depth=3) — croît vers les adresses basses
[walk_stack 0] marker=0  @ 0x7fff383e17c4
[walk_stack 1] marker=10 @ 0x7fff383e1794  (-48 o)
[walk_stack 2] marker=20 @ 0x7fff383e1764  (-48 o)
[walk_stack 3] marker=30 @ 0x7fff383e1734  (-48 o)
[dump_frame 3] local_buf @ 0x7fff383e16f0 ("D") | local_int @ 0x7fff383e16e4 (103) | p_local = 0x7fff383e16e4
```

* **Durée de vie :** automatique (chaque `marker` vit le temps de son instance de `walk_stack` ; `local_int`, `local_buf` et `p_local` disparaissent à la fin de chaque `dump_frame`). Les chaînes littérales (`"enter"`, `"exit"`) résident dans `.rodata` (durée de vie statique).
* **Alias :** `p_local` est un alias local de `local_int` (`p_local == &local_int`) qui ne sort jamais de son cadre (aucun pointeur pendant).
* **Tas :** 0 octet alloué par le code utilisateur *(hors tampon interne éventuel de `glibc`/`printf`)*.

---

### 2.2 `heap_example.c` — Allocations sur le tas et fuite par perte de propriété

Le tas croît vers les **adresses hautes**, avec un écart constant de `+0x20` (**32 octets**), correspondant à la taille minimale d'un chunk `ptmalloc2` (`glibc`) sur architecture 64 bits.

| Bloc tas | Adresse observée | Taille demandée / Chunk | Contenu | État en fin de `main()` |
| :--- | :--- | :---: | :--- | :--- |
| `alice` (`struct Person`) | `0x63ae18d67420` | 16 o (`8 + 4 + 4` padding) / 32 o | `name=0x...7440`, `age=30` | Libéré via `person_free_partial(alice)` |
| `alice->name` | `0x63ae18d67440` | 6 o / 32 o | `"Alice\0"` | **Fuite mémoire** (*definitely lost*) |
| `bob` (`struct Person`) | `0x63ae18d67460` | 16 o (`8 + 4 + 4` padding) / 32 o | `name=0x...7480`, `age=41` | Libéré (`free(bob)`) |
| `bob->name` | `0x63ae18d67480` | 4 o / 32 o | `"Bob\0"` | Libéré (`free(bob->name)`) |

```text
État en fin de main() :
PILE: alice = 0x63ae18d67420 (dangling) --X--> TAS: 0x63ae18d67420 [LIBÉRÉ]
                                                    0x63ae18d67440 [6 o "Alice\0"] <-- ORPHELIN (fuite)
PILE: bob   = 0x63ae18d67460 (dangling) --X--> TAS: 0x63ae18d67460 [LIBÉRÉ] / 0x63ae18d67480 [LIBÉRÉ]
```

* **Durée de vie :** les variables locales de `person_new()` sont dépilées au `return`, tandis que les 4 blocs sur le tas persistent. `bob` et `bob->name` sont libérés dans le bon ordre ; `alice->name` reste alloué jusqu'à la fin du processus.
* **Alias et perte de propriété :** dans `person_free_partial(Person *p)`, le paramètre `p` sur la pile est un alias d'`alice` (`0x63ae18d67420`). `free(p)` détruit la structure avant de libérer `p->name`, supprimant la seule référence vers `0x63ae18d67440` (*leak due to lost ownership*).

---

### 2.3 `aliasing_example.c` — Aliasing de pointeurs et *Use-After-Free*

`a` et `b` sont deux variables distinctes sur la pile (`&a != &b`) contenant toutes deux l'adresse `0x5f5c9f3c0420` (bloc tas de **20 octets**, `int[5]`). L'appel `free(a)` met fin à la durée de vie de l'objet sans modifier `a` ni `b` : les deux deviennent simultanément des pointeurs pendants (*dangling pointers*).

| Offset dans `0x5f5c9f3c0420` | Avant `free(a)` | Après `free(a)` (`tcache` `glibc`) | Action du programme |
| :--- | :---: | :--- | :--- |
| `+0..+7` (`b[0]`, `b[1]`) | `0`, `11` | `tcache_entry->next` | Non accédé |
| `+8..+11` (`b[2]`) | `22` | `tcache_entry->key` (bas) | **Lecture invalide** → `-738082945` |
| `+12..+15` (`b[3]`) | `33` | `tcache_entry->key` (haut) | **Écriture invalide (`1234`)** puis relecture → corruption du `tcache` |
| `+16..+19` (`b[4]`) | `44` | Donnée résiduelle | Non accédé |

* **Pourquoi aucun crash en exécution native ?** `free(a)` rend ce petit bloc au cache interne de l'allocateur `glibc` (`tcache`) sans démaper la page virtuelle auprès du noyau OS (`munmap()`). Les lectures et écritures hors durée de vie frappent une page toujours mappée en lecture/écriture : le bug corrompt les métadonnées internes du tas mais passe silencieusement sans Valgrind.

---

### 2.4 `crash_example.c` — Déréférencement déterministe de `NULL`

| Étape | État de `nums` (pile) | État du tas | Résultat |
| :--- | :---: | :--- | :--- |
| `allocate_numbers(0)` | `0x0` (`NULL`) | 0 o (`if (n <= 0) return NULL;`) | Retour immédiat avant tout appel à `malloc` |
| `nums[0] = 42;` | `0x0` (`NULL`) | 0 o | Écriture de 4 o à `0x0` (page zéro non mappée) → **`SIGSEGV`** |

```text
PILE (main): n = 0, nums = 0x0 --X--> PAGE 0x0 (NON MAPPÉE PAR LE NOYAU) ==> SIGSEGV immédiat
```

* Les instructions suivantes (`printf("  nums[0]=%d\n", nums[0]);` et `free(nums);`) ne sont jamais exécutées.

---

## 3. Audit critique : erreurs de l'IA et corrections apportées

| Programme | Erreur ou imprécision initiale de l'IA | Pourquoi c'était incorrect / incomplet | Correction apportée après vérification |
| :--- | :--- | :--- | :--- |
| **`aliasing_example.c`** | Affirmer que `free(a)` rend la mémoire à l'OS et fait planter `b[2]` / `b[3] = 1234` (ou laisse `b[2]` intact à `22`). | Contredit par l'exécution réelle : `./aliasing` termine sans crash (`exit 0`) et affiche `b[2]=-738082945`. | Distinction entre fin de durée de vie logique (*use-after-free*) et état de la page virtuelle (toujours mappée), plus explication de l'écrasement des 16 premiers octets (`b[0..3]`) par la structure `tcache_entry` (`next` + `key`) de `glibc`. |
| **`stack_example.c` & `crash_example.c`** | Affirmer que le tas du processus est strictement vide (`0` octet). | Imprécis : le code utilisateur n'appelle pas `malloc`, mais `glibc` effectue ses propres allocations internes sur le tas (tampon `stdout` de `printf` / structures `tcache`), visibles via l'offset initial `...420` dans `./heap` et `./aliasing`. | Formulation précisée en **« 0 octet alloué par le programme utilisateur (hors allocations internes `glibc`) »**. |
| **`stack_example.c`** | Représenter `dump_frame("enter")` et `dump_frame("exit")` comme s'empilant à des adresses différentes. | Contredit par `./stack` : à profondeur égale, `&local_int` est identique à l'entrée et à la sortie (`0x7fff383e16e4` à `depth=3`). | Correction des schémas pour montrer le dépilement immédiat de chaque `dump_frame` et la réutilisation exacte de son emplacement sur la pile. |
| **`heap_example.c`** | Calculer `sizeof(Person)` à 12 octets (`8 + 4`). | Faux sur x86-64 : l'alignement sur le plus grand membre (`char *`, 8 octets) impose 4 octets de padding après `int age`. | Corrigé à **16 octets** (`8 + 4 + 4` de padding), stockés dans un chunk `glibc` minimal de **32 octets**. |

---

## 4. Résumé

* **Pile vs Tas (comportements opposés) :** la pile (`0x7fff...`) est gérée automatiquement, croît vers les adresses basses (`-48` o par appel récursif dans `./stack`) et réutilise immédiatement les cadres dépilés ; le tas (`0x63ae...`, `0x5f5c...`) est géré manuellement, croît vers les adresses hautes par chunks minimaux de 32 octets (`glibc`) et survit au retour des fonctions.
* **Durée de vie et propriété :** un bloc sur le tas sans pointeur atteignable devient une fuite définitive (`alice->name`, 6 o perdus dans `./heap` car la structure parente a été libérée avant son champ). À l'inverse, un accès à `0x0` (`./crash`) provoque un arrêt matériel immédiat (`SIGSEGV`) car la page zéro n'est jamais mappée.
* **Danger de l'aliasing silencieux :** dans `./aliasing`, libérer `a` invalide instantanément son alias `b`, mais l'exécution native ne plante pas ; elle lit et corrompt les métadonnées internes du `tcache` `glibc` (`b[2]=-738082945`, `b[3]=1234`), illustrant pourquoi l'absence de crash ne prouve jamais l'absence de bug mémoire.
* **Vérification expérimentale indispensable :** confronter les hypothèses de l'IA aux adresses réelles a permis de corriger 4 erreurs concrètes (padding de `struct Person`, réutilisation de pile, allocations internes `glibc`, et comportement du `tcache` après `free()`).
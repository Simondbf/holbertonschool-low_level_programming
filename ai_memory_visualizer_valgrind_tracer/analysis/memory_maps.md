# Cartographies mémoire — Tâche 0 (AI Memory Visualizer)

> Analyse de la mémoire (pile, tas, durées de vie et alias de pointeurs) des quatre programmes de `programs/` sur Linux x86-64 (`sizeof(int) == 4`, `sizeof(void *) == 8`, `sizeof(Person) == 16`), vérifiée par exécution réelle sans Valgrind et sans aucune modification du code source.

## Vue d'ensemble

| Programme | Pile (Stack) | Tas (Heap utilisateur) | Alias de pointeurs | Bug / Comportement observé |
| :--- | :--- | :--- | :--- | :--- |
| **`stack_example.c`** | 4 cadres `walk_stack` (`-48` o/appel) + `dump_frame` réutilisé | 0 o | `p_local == &local_int` | Aucun bug — réutilisation exacte des adresses de pile |
| **`heap_example.c`** | `alice`, `bob` (`main`), `p` (`person_free_partial`) | 4 blocs (`42` o demandés, `32` o/chunk `glibc`) | `p == alice` lors de `person_free_partial` | **Fuite mémoire de 6 o** (`"Alice\0"`, *definitely lost*) |
| **`aliasing_example.c`** | `n`, `a`, `b` (`main`) | 1 bloc de `20` o (`int[5]`) | `b == a == 0x5f5c9f3c0420` | **Use-after-free** : `b[2]=-738082945` (métadonnées `tcache`), `b[3]=1234` (corruption), pas de crash |
| **`crash_example.c`** | `n`, `nums` (`main`) | 0 o (`malloc` non appelé) | Aucun | Déréférencement `NULL` (`0x0`) → **`SIGSEGV` immédiat** |

---

## 1. `stack_example.c` — Récursion et cadres de pile

La pile croît vers les **adresses basses** (`-0x30` = `-48` octets par appel récursif de `walk_stack`). Chaque appel à `dump_frame` est dépilé immédiatement avant l'instruction suivante, ce qui explique que `[enter]` et `[exit]` occupent exactement les mêmes adresses à profondeur égale.

| Profondeur | `&marker` (`walk_stack`) | Écart | `&local_int` (`[enter]` et `[exit]`) | `p_local` (alias) | `local_buf` |
| :--- | :--- | :--- | :--- | :--- | :--- |
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

* **Durée de vie :** automatique (limitée au cadre de chaque fonction). Les chaînes littérales (`"enter"`, `"exit"`) résident dans `.rodata` (durée de vie statique).
* **Alias :** `p_local` pointe sur `local_int` dans le même cadre et disparaît en même temps que lui (aucun pointeur pendant).
* **Tas :** `0` octet alloué par le programme C *(hors allocations internes éventuelles de `glibc`/`printf`)*.

---

## 2. `heap_example.c` — Allocations sur le tas et fuite mémoire

Le tas croît vers les **adresses hautes**, avec un écart constant de `+0x20` (`32` octets, taille minimale d'un chunk `glibc` sur 64 bits).

| Bloc tas | Adresse observée | Taille demandée / Chunk | Contenu | État en fin de `main()` |
| :--- | :--- | :--- | :--- | :--- |
| `alice` (`Person`) | `0x63ae18d67420` | `16` o (`8 + 4 + 4` padding) / `32` o | `name=0x...7440`, `age=30` | Libéré via `person_free_partial(alice)` |
| `alice->name` | `0x63ae18d67440` | `6` o / `32` o | `"Alice\0"` | **Fuite mémoire** (*definitely lost*) |
| `bob` (`Person`) | `0x63ae18d67460` | `16` o (`8 + 4 + 4` padding) / `32` o | `name=0x...7480`, `age=41` | Libéré (`free(bob)`) |
| `bob->name` | `0x63ae18d67480` | `4` o / `32` o | `"Bob\0"` | Libéré (`free(bob->name)`) |

```text
État en fin de main() :
PILE: alice = 0x63ae18d67420 (dangling) --X--> TAS: 0x63ae18d67420 [LIBÉRÉ]
                                                    0x63ae18d67440 [6 o "Alice\0"] <-- ORPHELIN (fuite)
PILE: bob   = 0x63ae18d67460 (dangling) --X--> TAS: 0x63ae18d67460 [LIBÉRÉ] / 0x63ae18d67480 [LIBÉRÉ]
```

* **Durée de vie :** les variables locales de `person_new()` sont dépilées au `return`, mais les 4 blocs tas persistent. `bob` et `bob->name` vivent jusqu'aux `free()` explicites ; `alice->name` reste alloué jusqu'à la fin du processus.
* **Alias :** dans `person_free_partial(Person *p)`, `p` (pile) est un alias d'`alice` (`0x63ae18d67420`). `free(p)` libère la structure sans libérer `p->name`, rendant `0x63ae18d67440` inatteignable.

---

## 3. `aliasing_example.c` — Aliasing et *Use-After-Free*

`a` et `b` sont deux variables distinctes sur la pile (`&a != &b`) contenant toutes deux `0x5f5c9f3c0420` (bloc de `20` octets sur le tas). Après `free(a)`, les deux deviennent simultanément des pointeurs pendants (*dangling pointers*).

| Offset dans le bloc (`0x5f5c9f3c0420`) | Avant `free(a)` | Après `free(a)` (`tcache` `glibc`) | Action du programme |
| :--- | :--- | :--- | :--- |
| `+0..+7` (`b[0]`, `b[1]`) | `0`, `11` | `tcache_entry->next` | Non accédé |
| `+8..+11` (`b[2]`) | `22` | `tcache_entry->key` (bas) | **Lecture invalide** → `-738082945` |
| `+12..+15` (`b[3]`) | `33` | `tcache_entry->key` (haut) | **Écriture invalide (`1234`)** → corruption du `tcache` |
| `+16..+19` (`b[4]`) | `44` | Donnée résiduelle | Non accédé |

* **Pourquoi aucun crash sans Valgrind ?** `free(a)` renvoie le bloc au cache interne de `glibc` (`tcache`), sans appeler `munmap()` auprès du noyau. La page reste mappée en lecture/écriture, donc l'accès passe silencieusement malgré le comportement indéfini (*Undefined Behavior*).

---

## 4. `crash_example.c` — Déréférencement de `NULL`

| Étape | État de `nums` (pile) | État du tas | Résultat |
| :--- | :--- | :--- | :--- |
| `allocate_numbers(0)` | `0x0` (`NULL`) | `0` o alloué (`if (n <= 0) return NULL;`) | Retour immédiat avant `malloc` |
| `nums[0] = 42;` | `0x0` (`NULL`) | `0` o alloué | Écriture de 4 o à `0x0` (page zéro non mappée) → **`SIGSEGV`** |

```text
PILE (main): n = 0, nums = 0x0 --X--> PAGE 0x0 (NON MAPPÉE) ==> SIGSEGV immédiat
```

* Les lignes `printf("  nums[0]=%d\n", nums[0]);` et `free(nums);` ne sont jamais atteintes.

---

## 5. Audit critique : erreurs de l'IA et corrections apportées

| Programme | Affirmation initiale de l'IA (erronée ou imprécise) | Vérification réelle et correction |
| :--- | :--- | :--- |
| `aliasing_example.c` | `free(a)` rend la mémoire à l'OS et fait planter `b[2]` / `b[3] = 1234` (ou laisse `b[2]` intact à `22`). | **Faux.** `./aliasing` termine sans crash (`exit 0`) et affiche `b[2]=-738082945` : la page reste mappée et `glibc` écrase les 16 premiers octets (`b[0..3]`) avec les métadonnées `tcache_entry` (`next` + `key`). |
| `stack_example.c` & `crash_example.c` | « Le tas est strictement vide (0 octet) dans le processus. » | **Imprécis.** Il y a `0` octet alloué par le *code utilisateur*, mais `glibc` effectue ses propres allocations internes sur le tas (ex. tampon `stdout` de `printf` / structures `tcache`), visibles via l'offset initial `...420` dans `./heap` et `./aliasing`. |
| `stack_example.c` | Les appels `dump_frame("enter")` et `dump_frame("exit")` s'empilent à des adresses différentes. | **Faux.** À profondeur égale, `&local_int` est identique à l'entrée et à la sortie (`0x7fff383e16e4` à `depth=3`), prouvant le dépilement et la réutilisation immédiate du cadre. |
| `heap_example.c` | `sizeof(Person)` vaut `12` octets (`8 + 4`). | **Faux.** Sur x86-64, l'alignement sur 8 octets ajoute `4` octets de padding après `int age` → `sizeof(Person) == 16` octets (alloués dans un chunk `glibc` de `32` octets). |

---

## 6. Mini synthèse de ce qui a été fait

* **Compilation des 4 programmes** de `programs/` (`stack`, `heap`, `aliasing`, `crash`) sans modifier le code source, en contournant le blocage `-Werror=use-after-free` de GCC sur `aliasing_example.c` via `-Wno-error=use-after-free`.
* **Exécution réelle sans Valgrind** de chaque binaire et relevé des adresses effectives (`0x7fff...` sur la pile, `0x63ae...` / `0x5f5c...` sur le tas, et `SIGSEGV` sur `./crash`).
* **Cartographie complète de la mémoire** pour les 4 cas : empilement/dépilement récursif, chunks du tas et fuite mémoire, aliasing et *use-after-free* avec corruption `tcache`, et déréférencement `NULL`.
* **Vérification critique des réponses de l'IA** : identification, preuve par l'exécution et correction de 4 erreurs ou imprécisions (comportement de `free()`/`tcache`, allocations internes `glibc`, réutilisation des cadres de pile, padding de `struct Person`).
* **Production du rapport final** structuré en tableaux Markdown dans `analysis/memory_maps.md`, prêt pour le dépôt GitHub.
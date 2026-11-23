# Analyse Valgrind — Tâche 1 (Valgrind & AI Memory Tracer)

* **Outil :** Valgrind Memcheck 3.26.0 (Linux x86-64)
* **Cibles :** `stack`, `heap`, `aliasing`, `crash` (compilés depuis `programs/` sans modification du code source)
* **Référence croisée :** Cartographies mémoire de la Tâche 0 (`analysis/memory_maps.md`)
* **Commande exécutée :**
  ```bash
  valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./<programme>
  ```

---

## 1. Tableau de synthèse global

| Programme | `total heap usage` | Erreurs (`ERROR SUMMARY`) | Fuites mémoire (`LEAK SUMMARY`) | Lectures / Écritures invalides | Mémoire non initialisée | Diagnostic principal |
| :--- | :--- | :---: | :--- | :---: | :---: | :--- |
| **`stack_example.c`** | 1 alloc, 1 free (1 024 o) | **0** | 0 o | 0 | 0 | Aucune erreur (tampon `stdout` `glibc` libéré normalement) |
| **`heap_example.c`** | 5 allocs, 4 frees (1 066 o) | **1** | **6 o `definitely lost`** | 0 | 0 | Fuite mémoire par perte de propriété (*leak due to lost ownership*) |
| **`aliasing_example.c`** | 2 allocs, 2 frees (1 044 o) | **3** | 0 o | **3** (2 reads, 1 write) | 0 | *Use-after-free* via alias de pointeur (`b` pendant après `free(a)`) |
| **`crash_example.c`** | 1 alloc, 0 free (1 024 o) | **1** (+ `SIGSEGV`) | 0 o perdus (`1 024 o still reachable` `glibc`) | **1** (write à `0x0`) | 0 | Déréférencement déterministe de pointeur `NULL` |

> **Vérification de la mémoire non initialisée (`--track-origins=yes`) :** **0 erreur** sur les quatre programmes. Dans `stack_example.c`, les octets `local_buf[2..15]` restent non initialisés sur la pile, mais ne sont jamais lus ni utilisés dans un branchement ; Memcheck ne signale donc aucune violation.

---

## 2. Analyse détaillée par programme

### 2.1 `stack_example.c` — Aucune erreur détectée

| Indicateur Valgrind | Valeur observée | Interprétation mémoire |
| :--- | :--- | :--- |
| `total heap usage` | `1 allocs, 1 frees, 1,024 bytes` | Aucun `malloc` dans le code C. L'unique bloc de 1 024 octets correspond au tampon interne `stdout` alloué par `glibc` au premier `printf()` et libéré proprement à la sortie de `main()`. |
| `in use at exit` | `0 bytes in 0 blocks` | Aucune fuite mémoire. |
| `ERROR SUMMARY` | `0 errors from 0 contexts` | Tous les accès sur la pile respectent la durée de vie de leur cadre. |

* **Cohérence avec la Tâche 0 :** sous Valgrind, la pile croît toujours vers les adresses basses par pas exacts de `0x30` (**48 octets** par appel récursif de `walk_stack` : `&marker` passe de `0x1ffefffe14` à `0x1ffefffde4`, `0x1ffefffdb4`, puis `0x1ffefffd84`), et chaque cadre `dump_frame` est réutilisé à l'identique entre `[enter]` et `[exit]` (`&local_int = 0x1ffefffdc4` à `depth=0`).

---

### 2.2 `heap_example.c` — Fuite mémoire par perte de propriété

#### Décompte exact du tas (`1 066` octets = 5 allocations)
| Bloc | Taille | Origine | Statut à la sortie |
| :--- | :---: | :--- | :--- |
| Tampon `stdout` (`glibc`) | 1 024 o | Premier `printf()` | Libéré par `libc` à la sortie |
| `alice` (`struct Person`) | 16 o (`8 + 4 + 4` padding) | `person_new` (`heap_example.c:14`) | Libéré par `person_free_partial(alice)` |
| `alice->name` (`"Alice\0"`) | **6 o** | `person_new` (`heap_example.c:21` via `main:51`) | **Non libéré (`definitely lost`)** |
| `bob` (`struct Person`) | 16 o (`8 + 4 + 4` padding) | `person_new` (`heap_example.c:14`) | Libéré par `free(bob)` (`main:67`) |
| `bob->name` (`"Bob\0"`) | 4 o | `person_new` (`heap_example.c:21`) | Libéré par `free(bob->name)` (`main:66`) |

#### Erreur rapportée par Valgrind
| Classification | Objet mémoire impliqué | Trace Valgrind | Violation de durée de vie / propriété |
| :--- | :--- | :--- | :--- |
| **Memory leak (`definitely lost`)** — 6 octets en 1 bloc | Chaîne `"Alice\0"` (`alice->name`, adresse `0x4a914d0`) | `malloc` → `person_new (heap_example.c:21)` → `main (heap_example.c:51)` | **Fuite par perte de propriété (*leak due to lost ownership*)** : à `main:69`, `person_free_partial(alice)` appelle `free(p)` (`heap_example.c:41`) sur la structure `Person` (`0x4a91480`) sans libérer `p->name` avant. Le seul pointeur permettant d'atteindre `0x4a914d0` est détruit avec la structure ; le bloc devient définitivement inatteignable. |

---

### 2.3 `aliasing_example.c` — *Use-After-Free* via alias de pointeur

* **Décompte du tas :** `1 044` octets (`1 024` o `stdout` + `20` o pour `int[5]`), 2 allocs, 2 frees → **0 fuite**.
* **Objet mémoire unique impliqué :** tableau dynamique `int[5]` de **20 octets** (`0x4a91480` à `0x4a91493`), alloué à `make_numbers (aliasing_example.c:12)` via `main:30`, puis libéré à `main:38` via `free(a)`.

#### Erreurs rapportées par Valgrind (`3 errors from 3 contexts`)
| # | Diagnostic Valgrind | Ligne C | Adresse et offset | Classification et violation de durée de vie |
| :-: | :--- | :--- | :--- | :--- |
| 1 | `Invalid read of size 4` | `aliasing_example.c:42` (`printf(..., b[2])`) | `0x4a91488` (`+8` o dans le bloc libéré) | ***Use-after-free* (lecture invalide)** : `free(a)` (ligne 38) a terminé la durée de vie du bloc ; `b`, alias de `a`, est un pointeur pendant (*dangling pointer*) qui déréférence de la mémoire libérée. |
| 2 | `Invalid write of size 4` | `aliasing_example.c:44` (`b[3] = 1234;`) | `0x4a9148c` (`+12` o dans le bloc libéré) | ***Use-after-free* (écriture invalide)** : écriture de 4 octets dans un objet tas dont la durée de vie est terminée (corruption mémoire). |
| 3 | `Invalid read of size 4` | `aliasing_example.c:45` (`printf(..., b[3])`) | `0x4a9148c` (`+12` o dans le bloc libéré) | ***Use-after-free* (lecture invalide)** : relecture de l'entier écrit hors durée de vie à l'offset `+12`. |

* **Pourquoi la ligne 40 (`printf("  after free(a): b=%p (dangling)\n", (void *)b);`) n'est pas signalée par Valgrind :**
  Selon la norme C, lire la *valeur* d'un pointeur pendant après `free()` est techniquement un comportement indéfini — c'est pourquoi GCC lève `-Wuse-after-free`. Au niveau machine, `b` reste cependant une variable locale initialisée de 8 octets sur la pile : tant que son adresse sur le tas n'est pas déréférencée, Memcheck ne détecte aucun accès mémoire invalide.

---

### 2.4 `crash_example.c` — Déréférencement de `NULL` (`SIGSEGV`)

#### Diagnostics rapportés par Valgrind (`1 errors from 1 contexts`)
| Diagnostic Valgrind | Ligne C | Adresse / Objet | Classification et cause exacte |
| :--- | :--- | :--- | :--- |
| `Invalid write of size 4` + **`SIGSEGV`** | `crash_example.c:32` (`nums[0] = 42;`) | `0x0` (`not stack'd, malloc'd or free'd`, hors région mappée) | **Déréférencement déterministe de `NULL`** : `allocate_numbers(0)` exécute `if (n <= 0) return NULL;` (`crash_example.c:9-10`) sans appeler `malloc`. `main()` tente d'écrire 4 octets à `0x0` (page zéro non mappée par le noyau) sans vérifier `if (!nums)`. |
| `1,024 bytes in 1 blocks are still reachable` | `crash_example.c:27` (`puts` ← `printf`) | Tampon `stdout` (`_IO_file_doallocate`) | **Allocation interne `glibc` toujours atteignable (non une fuite utilisateur)** : le `SIGSEGV` ligne 32 tue le processus avant les routines de nettoyage `libc`. Valgrind est factuellement exact et ne le compte pas dans `ERROR SUMMARY` par défaut (`--errors-for-leak-kinds=definite,possible`). |

* **Origine de `puts` dans la pile d'appels :** à `crash_example.c:27`, `printf("...\n")` ne contient aucun spécificateur de format ; GCC remplace automatiquement cet appel par `puts()`, produisant la trace `malloc` ← `_IO_file_doallocate` ← `puts (ioputs.c:40)` ← `main (crash_example.c:27)`.

---

## 3. Comparaison exécution native (`glibc`) vs Valgrind (`Memcheck`)

Croiser la Tâche 0 et la Tâche 1 met en évidence l'impact de l'allocateur de remplacement de Valgrind (`vg_replace_malloc.c`) :

| Observation | Exécution native (`glibc` / `ptmalloc2`) | Exécution sous Valgrind (`vg_replace_malloc.c`) | Explication technique |
| :--- | :--- | :--- | :--- |
| **Valeur lue dans `b[2]` (`./aliasing`)** | `-738082945` (aucun crash) | `22` (3 erreurs Memcheck) | En natif, `free(a)` place le bloc dans le `tcache` `glibc` et écrase les 16 premiers octets (`b[0..3]`) avec `tcache_entry` (`next` + `key`). Valgrind met le bloc en quarantaine sans écrire de clé `tcache`, tout en marquant ses octets comme inaccessibles dans la *shadow memory*. |
| **Pas entre blocs tas (`./heap`)** | `+0x20` (**32 octets**) | `+0x50` (**80 octets** : `0x4a91480` → `...4d0` → `...520` → `...570`) | `glibc` utilise une taille minimale de chunk de 32 octets sur 64 bits. Memcheck insère des **redzones** (zones de garde interdites) autour de chaque bloc pour détecter les débordements de tas. |
| **Tampon `stdout` (`1 024` o)** | Invisible (trahi par l'offset initial `...420`) | Visible explicitement (`1,024 bytes`) dans les 4 exécutables | Valgrind confirme au byte près l'allocation du tampon `stdout` de `glibc` et la taille `sizeof(Person) == 16` (`1 024 + 16 + 6 + 16 + 4 = 1 066` o dans `./heap`). |

---

## 4. Audit critique : erreurs et omissions de l'IA corrigées

| Programme | Erreur ou omission initiale de l'IA | Pourquoi c'était incorrect / incomplet | Correction apportée après vérification |
| :--- | :--- | :--- | :--- |
| **`crash_example.c`** | Interpréter `1,024 bytes still reachable` comme une fuite du programme utilisateur, puis le qualifier de « faux positif ». | Les deux formulations sont inexactes : ce n'est pas une fuite du code C (la trace pointe vers `_IO_file_doallocate` / `puts` ligne 27), et ce n'est pas non plus un faux positif de Valgrind puisque le bloc est réellement encore alloué et atteignable au moment du `SIGSEGV`. | Reclassé précisément en **allocation interne `glibc` toujours atteignable**, interrompue avant le nettoyage `libc` par le `SIGSEGV`, et exclue de `ERROR SUMMARY` par défaut. |
| **`aliasing_example.c`** | Supposer que Valgrind signale la ligne 40 (comme GCC `-Wuse-after-free`) ou que `b[2]` continue d'afficher `-738082945` sous Valgrind. | Contredit par la sortie réelle : Valgrind rapporte exactement **3 erreurs** (lignes 42, 44, 45) et affiche `b[2]=22`. | Distinction explicite entre l'analyse statique de GCC (norme C sur la ligne 40) et la détection dynamique de Memcheck (déréférencements effectifs uniquement), ainsi que le rôle de `vg_replace_malloc.c`. |
| **`crash_example.c` & `heap_example.c`** | Omissions au premier passage : absence d'explication sur `puts` (ligne 27), sur l'écart de 80 octets dans `./heap`, et sur le bilan de la mémoire non initialisée. | Un rapport incomplet laisse des incohérences apparentes entre le code source (`printf`), la Tâche 0 (`32` o) et la sortie Valgrind (`puts`, `80` o). | Ajout de l'optimisation GCC `printf` → `puts`, des *redzones* Valgrind (`0x50` = 80 o) et de la vérification explicite des valeurs non initialisées (`0` erreur). |

---

## 5. Résumé

* **Lien direct mémoire ↔ Valgrind :** chaque message Memcheck correspond à une cause concrète déjà identifiée dans les cartographies mémoire de la Tâche 0 : aucune erreur sur la pile (`./stack`), **1 fuite `definitely lost` de 6 octets** par perte de propriété (`./heap`), **3 accès *use-after-free*** via un pointeur pendant (`./aliasing`), et **1 écriture invalide à `0x0`** provoquant un `SIGSEGV` (`./crash`), avec **0 erreur de mémoire non initialisée**.
* **Validation chiffrée au byte près :** les totaux Valgrind (`1 024` o, `1 066` o, `1 044` o) confirment exactement `sizeof(Person) == 16`, la taille de chaque allocation utilisateur et le tampon interne `stdout` de `glibc`.
* **Compréhension des outils :** l'analyse a permis de distinguer le comportement de l'allocateur natif `glibc` (`tcache`, chunks de 32 o) de celui de Valgrind (`vg_replace_malloc.c`, quarantaine, *redzones* de 80 o), ainsi que la différence entre avertissement statique GCC et détection dynamique Memcheck.
* **Esprit critique :** la vérification systématique des piles d'appels a corrigé les raccourcis initiaux de l'IA (notamment sur le bloc `still reachable` de `./crash` et les différences natif/Valgrind sur `./aliasing`).
# Cartographies mémoire — AI Memory Visualizer

> Analyse synthétique de l'état mémoire (pile, tas, durées de vie et alias de pointeurs) des 4 programmes de `programs/`, vérifiée par exécution réelle sur Linux x86-64 (`glibc`, sans Valgrind, code non modifié).

---

## Synthèse globale

| Programme | Zone clé | Durée de vie / Alias | Comportement réel (`./binaire`) | Diagnostic mémoire |
| :--- | :--- | :--- | :--- | :--- |
| **`stack_example.c`** | Pile (`0x7fff...`, croît vers le bas) | Cadres automatiques ; alias `p_local == &local_int` | `exit 0` | Sains : 4 cadres récursifs (`-48` o/appel) + réutilisation exacte des cadres `dump_frame`. |
| **`heap_example.c`** | Tas (`0x63ae...`, croît vers le haut) | 4 blocs dynamiques ; alias `p == alice` lors du `free` | `exit 0` | **Fuite de 6 octets** (`"Alice\0"` orphelin après `person_free_partial`). |
| **`aliasing_example.c`** | Pile + Tas (`0x5f5c...`) | Alias `b == a` sur bloc de 20 o | `exit 0`, `b[2]=-738082945`, `b[3]=1234` | **Use-after-free** + corruption silencieuse des métadonnées `tcache` de `glibc`. |
| **`crash_example.c`** | Pile (`nums = 0x0`) | Aucun bloc tas alloué | **`Segmentation fault` (`SIGSEGV`)** | Écriture à `0x0` (page zéro non mappée par le noyau). |

*Repères x86-64 : `sizeof(int) = 4` o, `sizeof(void *) = 8` o, `sizeof(Person) = 16` o (`8 + 4 + 4` o de padding), chunk `glibc` minimal = `32` o (`0x20`). Chaînes littérales dans `.rodata` (durée de vie statique).*

---

## 1. `stack_example.c` — Récursion et cadres de pile

* **Tas :** `0` octet alloué par le programme C *(hors allocations internes éventuelles de `glibc`/`printf`)*.
* **Durée de vie :** `marker` vit pendant son appel `walk_stack(depth)` ; `local_int`, `local_buf` et `p_local` ne vivent que le temps d'un appel `dump_frame` (dépilé immédiatement).
* **Alias :** `p_local == &local_int` (alias local, détruit avec le cadre, jamais *dangling*).

| `depth` | `&marker` (`walk_stack`) | Écart | `&local_int` (`enter` **et** `exit`) | `local_buf` (`enter` **et** `exit`) |
| :---: | :---: | :---: | :---: | :---: |
| `0` | `0x7fff383e17c4` (`0`) | — | `0x7fff383e1774` (`100`) | `0x7fff383e1780` (`"A"`) |
| `1` | `0x7fff383e1794` (`10`) | `-48` o (`-0x30`) | `0x7fff383e1744` (`101`) | `0x7fff383e1750` (`"B"`) |
| `2` | `0x7fff383e1764` (`20`) | `-48` o (`-0x30`) | `0x7fff383e1714` (`102`) | `0x7fff383e1720` (`"C"`) |
| `3` | `0x7fff383e1734` (`30`) | `-48` o (`-0x30`) | `0x7fff383e16e4` (`103`) | `0x7fff383e16f0` (`"D"`) |

```text
PILE au pic ([enter] depth=3) — croît vers les adresses basses
[walk_stack 0] marker=0  @ ...17c4
[walk_stack 1] marker=10 @ ...1794  (-48 o)
[walk_stack 2] marker=20 @ ...1764  (-48 o)
[walk_stack 3] marker=30 @ ...1734  (-48 o)
[dump_frame 3] local_buf @ ...16f0 ("D") | local_int @ ...16e4 (103) | p_local = ...16e4
```

> **Point clé :** `[enter]` et `[exit]` ont les **mêmes adresses** à chaque profondeur, preuve que chaque cadre `dump_frame` est dépilé puis réutilisé.

---

## 2. `heap_example.c` — Tas et fuite mémoire volontaire

* **Durée de vie :** les variables locales de `person_new()` sont dépilées au `return`, mais les **4 blocs `malloc`** survivent sur le tas jusqu'au `free()` (ou la fin du processus).
* **Alias :** dans `person_free_partial(alice)`, le paramètre `p` (pile) est un **alias d'`alice`** (`0x63ae18d67420`). Libérer `p` invalide `alice` sans libérer `alice->name`.

| Bloc tas | Adresse réelle | Demandé / Chunk | Contenu | État en fin de `main()` |
| :--- | :---: | :---: | :--- | :--- |
| `alice` (`Person`) | `0x63ae18d67420` | `16` o / `32` o | `name=...7440`, `age=30` | Libéré (`person_free_partial`) |
| `alice->name` | `0x63ae18d67440` | `6` o / `32` o | `"Alice\0"` | **FUITE (*definitely lost*)** |
| `bob` (`Person`) | `0x63ae18d67460` | `16` o / `32` o | `name=...7480`, `age=41` | Libéré (`free(bob)`) |
| `bob->name` | `0x63ae18d67480` | `4` o / `32` o | `"Bob\0"` | Libéré (`free(bob->name)`) |

```text
Fin de main() :
PILE: alice = ...7420 (dangling) --X--> TAS: ...7420 [LIBÉRÉ]
                                             ...7440 [6 o "Alice\0"] <-- ORPHELIN (fuite)
PILE: bob   = ...7460 (dangling) --X--> TAS: ...7460 [LIBÉRÉ] / ...7480 [LIBÉRÉ]
```

---

## 3. `aliasing_example.c` — Aliasing et *Use-After-Free*

* **Alias :** `a` et `b` sont 2 variables distinctes sur la pile (`&a != &b`), mais pointent vers le même bloc tas de `20` o (`0x5f5c9f3c0420`).
* **Durée de vie :** `free(a)` met fin à la vie du bloc mais laisse `a` et `b` inchangés (**tous deux *dangling***).
* **Pourquoi pas de crash ?** `free(a)` rend le chunk au `tcache` de `glibc` (page toujours mappée en lecture/écriture par l'OS), sans `munmap`.

| Index | Offset | Avant `free(a)` | Après `free(a)` (`glibc` `tcache_entry`) | Action dans `main()` |
| :---: | :---: | :---: | :--- | :--- |
| `b[0..1]` | `+0..+7` | `0`, `11` | `tcache->next` (pointeur libre) | Non accédé |
| `b[2]` | `+8..+11` | `22` | `tcache->key` (bas) | **Lecture invalide** → `-738082945` |
| `b[3]` | `+12..+15` | `33` | `tcache->key` (haut) | **Écriture invalide (`1234`)** → corruption `tcache` |
| `b[4]` | `+16..+19` | `44` | `44` (résiduel) | Non accédé |

---

## 4. `crash_example.c` — Déréférencement de `NULL`

* **Tas :** `0` octet alloué par le programme (`if (n <= 0) return NULL;` s'exécute avant `malloc`).
* **Crash :** `nums[0] = 42;` écrit `4` octets à l'adresse virtuelle `0x0`. La page zéro n'étant jamais mappée par Linux, le noyau envoie immédiatement **`SIGSEGV`** (`printf` et `free` ne sont jamais atteints).

```text
PILE (main): n = 0, nums = 0x0 --X--> PAGE 0x0 (NON MAPPÉE) ==> SIGSEGV immédiat
```

---

## 5. Erreurs de l'IA détectées et corrigées

| # | Programme | Erreur initiale de l'IA | Réalité observée | Correction apportée |
| :---: | :--- | :--- | :--- | :--- |
| **1** | `aliasing_example.c` | A prédit soit que `b[2]`/`b[3]` restent inchangés, soit que le programme plante après `free(a)`. | `./aliasing` affiche `b[2]=-738082945`, écrit `b[3]=1234` et termine avec `exit 0`. | Prise en compte de `tcache_entry` (`glibc` 2.29+, 16 o) : `free(a)` écrase `+8..+15` avec `tcache->key` sur une page restée mappée. |
| **2** | `stack_example.c` & `crash_example.c` | A affirmé « Tas totalement vide (0 octet) » de façon absolue. | Dans `./heap` et `./aliasing`, le 1er `malloc` démarre à `...420` (allocations internes `glibc` préalables). | Distinction explicite entre **0 octet alloué par le programme C** et les allocations internes de `glibc` (`tcache` / `stdout`). |
| **3** | `stack_example.c` & `heap_example.c` | Raccourcis classiques : adresses distinctes entre `[enter]`/`[exit]`, ou `sizeof(Person) == 12` o. | Adresses identiques entre `[enter]`/`[exit]` (`-48` o/appel) ; pas de `32` o entre chunks sur le tas. | Intégration des adresses réelles, des **16 octets** de `Person` (`4` o de padding) et du chunk minimal de **32 octets**. |
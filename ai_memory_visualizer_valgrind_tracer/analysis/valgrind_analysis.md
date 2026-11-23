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

| Programme | Mécanisme mémoire fautif (explication autonome) | Classification de l'erreur | Preuve Memcheck (`ERROR` / `LEAK`) |
| :--- | :--- | :--- | :--- |
| **`stack_example.c`** | Toutes les variables résident sur la pile automatique et sont accédées uniquement pendant l'exécution de leur cadre ; `local_buf[2..15]` n'est jamais lu. | **Aucune erreur** | `0 errors`, `0 bytes in use at exit` (`1 024` o `stdout` libérés par `glibc`) |
| **`heap_example.c`** | `person_new()` transfère à `main()` la propriété d'une structure `Person` et d'une chaîne `"Alice\0"`, mais `person_free_partial()` détruit la structure avant sa chaîne. | **Fuite mémoire par perte de propriété** (*leak due to lost ownership*) | `1 error`, **`6 bytes definitely lost`** (`heap_example.c:21` via `main:51`) |
| **`aliasing_example.c`** | `a` (propriétaire) et `b` (alias non propriétaire) pointent vers le même bloc de 20 o ; `free(a)` termine sa durée de vie, mais `b` lit et écrit encore dedans. | ***Use-after-free*** (2 lectures invalides, 1 écriture invalide) | `3 errors` (lignes `42`, `44`, `45`), `0` fuite |
| **`crash_example.c`** | `allocate_numbers(0)` retourne `NULL` avant tout `malloc`, et `main()` écrit 4 octets à l'adresse `0x0` sans vérifier le pointeur. | **Déréférencement de pointeur `NULL`** (*invalid write* à `0x0`) | `1 error` (ligne `32`) + **`SIGSEGV`** (`1 024 o still reachable` `glibc`) |

> **Vérification de la mémoire non initialisée (`--track-origins=yes`) :** **0 erreur** sur les quatre programmes. Aucune branche conditionnelle, adresse mémoire ou sortie console ne dépend d'une valeur non initialisée.

---

## 2. Analyse détaillée par programme (raisonnement mémoire → preuve Valgrind)

### 2.1 `stack_example.c` — Gestion saine de la pile automatique

* **Raisonnement mémoire autonome :**
  Le programme n'effectue aucune allocation dynamique (`malloc`) et n'utilise que des variables locales sur la pile (`marker` dans `walk_stack`, `local_int`, `local_buf` et `p_local` dans `dump_frame`). Aucune adresse locale n'est retournée ni conservée après la fin d'un appel : l'alias `p_local` (`&local_int`) reste strictement confiné à son cadre. Bien que les octets `local_buf[2..15]` ne soient jamais initialisés, ils ne sont jamais lus non plus.
* **Confirmation par Valgrind :**
  * `ERROR SUMMARY : 0 errors from 0 contexts` (aucun accès hors durée de vie, aucune lecture de valeur non initialisée).
  * `total heap usage : 1 allocs, 1 frees, 1,024 bytes` — cet unique bloc correspond au tampon interne `stdout` alloué par `glibc` lors du premier `printf()` et libéré proprement à la sortie de `main()`.
  * Sous Valgrind, chaque appel récursif décale toujours la pile de `-0x30` (**-48 octets** : `&marker` passe de `0x1ffefffe14` à `0x1ffefffd84`), et `[enter]` / `[exit]` réutilisent exactement les mêmes adresses (`&local_int = 0x1ffefffdc4` à `depth=0`).

---

### 2.2 `heap_example.c` — Fuite mémoire par perte de propriété (*Leak Due to Lost Ownership*)

* **Raisonnement mémoire autonome :**
  `person_new()` alloue deux blocs distincts sur le tas — une `struct Person` de 16 octets (`8` o pour `char *name` + `4` o pour `int age` + `4` o de padding d'alignement) et une chaîne `name` (`strlen + 1` octets) — puis **transfère la propriété des deux blocs à `main()`**. Pour respecter ce contrat de propriété, l'appelant doit libérer le bloc enfant (`p->name`) **avant** le bloc parent (`p`).
  * Pour `bob`, `main()` respecte l'ordre (`free(bob->name)` ligne 66, puis `free(bob)` ligne 67).
  * Pour `alice`, `main()` appelle `person_free_partial(alice)` (ligne 69), qui exécute uniquement `free(p)` (ligne 41). En libérant la structure parente sans avoir libéré `p->name` (`"Alice\0"`, 6 octets), le programme détruit l'unique pointeur permettant encore d'atteindre cette chaîne : le bloc devient orphelin jusqu'à la fin du processus.
* **Confirmation par Valgrind :**
  * **Classification :** `Memory leak (definitely lost)` — `6 bytes in 1 blocks` (`0x4a914d0`), alloués à `heap_example.c:21` (`person_new`) via `main:51`.
  * **Comptabilité du tas (`1 066` octets = 5 allocs, 4 frees) :** `1 024` o (`stdout` `glibc`) + `16` o (`alice`) + `6` o (`alice->name`) + `16` o (`bob`) + `4` o (`bob->name`).
  * **Écart entre blocs (`80` o vs `32` o en natif) :** sous Valgrind, les adresses utilisateur (`0x4a91480`, `0x4a914d0`, `0x4a91520`, `0x4a91570`) sont espacées de `0x50` (**80 octets**) au lieu de `0x20` (**32 octets**) sous `glibc`, car l'allocateur Memcheck insère des *redzones* (zones de garde inaccessibles) autour de chaque bloc.

---

### 2.3 `aliasing_example.c` — *Use-After-Free* via alias non propriétaire

* **Raisonnement mémoire autonome :**
  `make_numbers()` (ligne 12) alloue un tableau dynamique de 5 entiers (`20` octets) sur le tas et **transfère sa propriété à `main()` via `a`** (ligne 30). L'affectation `b = a;` (ligne 31) crée un **alias non propriétaire** pointant vers la même région mémoire. Lorsque `main()` appelle `free(a)` (ligne 38), la durée de vie du bloc de 20 octets prend immédiatement fin, transformant simultanément `a` et `b` en **pointeurs pendants (*dangling pointers*)**. Toute lecture ou écriture ultérieure via `b` accède à une mémoire dont la durée de vie est expirée (*use-after-free*).
* **Confirmation par Valgrind (`3 errors from 3 contexts`, `0` fuite sur `1 044` o) :**
  * **Erreur 1 (`aliasing_example.c:42` — `printf(..., b[2])`) :** `Invalid read of size 4` à `0x4a91488` (offset `+8` octets dans le bloc libéré `0x4a91480..0x4a91493`).
  * **Erreur 2 (`aliasing_example.c:44` — `b[3] = 1234;`) :** `Invalid write of size 4` à `0x4a9148c` (offset `+12` octets dans le bloc libéré).
  * **Erreur 3 (`aliasing_example.c:45` — `printf(..., b[3])`) :** `Invalid read of size 4` à `0x4a9148c` (relecture à l'offset `+12` octets).
  * **Pourquoi la ligne 40 (`printf("... b=%p", (void *)b)`) n'est pas signalée :** lire la *valeur* d'un pointeur pendant est un comportement indéfini en C (d'où l'avertissement statique GCC `-Wuse-after-free`), mais au niveau machine `b` est une variable locale initialisée de 8 octets sur la pile : Memcheck ne signale que les déréférencements effectifs vers le tas libéré.

---

### 2.4 `crash_example.c` — Déréférencement déterministe de `NULL`

* **Raisonnement mémoire autonome :**
  Dans `main()`, `n` vaut `0` (ligne 25). L'appel `allocate_numbers(0)` (ligne 30) entre dans la garde `if (n <= 0) return NULL;` (lignes 9-10) et retourne immédiatement `0x0` sans jamais appeler `malloc()` (ligne 12) ni transférer de propriété. À la ligne 32, `main()` exécute `nums[0] = 42;` sans vérifier `if (!nums)` : le programme tente d'écrire 4 octets à l'adresse virtuelle `0x0` (page zéro volontairement non mappée par le noyau Linux), ce qui déclenche une faute de page matérielle et l'arrêt immédiat par `SIGSEGV`.
* **Confirmation par Valgrind (`1 errors from 1 contexts`) :**
  * **Erreur unique (`crash_example.c:32`) :** `Invalid write of size 4` à `Address 0x0 is not stack'd, malloc'd or (recently) free'd`, suivie de `Process terminating with default action of signal 11 (SIGSEGV)`.
  * **Bloc `1,024 bytes still reachable` (`crash_example.c:27`) :** la pile d'appels (`malloc` ← `_IO_file_doallocate` ← `puts` ← `main:27`, GCC optimisant `printf("...\n")` sans format en `puts()`) prouve qu'il s'agit du tampon `stdout` interne de `glibc`. Le `SIGSEGV` ligne 32 interrompt le processus avant le nettoyage `libc` de fin de programme ; Valgrind est factuellement exact et ne le compte pas dans `ERROR SUMMARY` par défaut (`--errors-for-leak-kinds=definite,possible`).

---

## 3. Comparaison exécution native (`glibc`) vs Valgrind (`Memcheck`)

| Observation | Exécution native (`glibc` / `ptmalloc2`) | Exécution sous Valgrind (`vg_replace_malloc.c`) | Explication technique |
| :--- | :--- | :--- | :--- |
| **Valeur lue dans `b[2]` (`./aliasing`)** | `-738082945` (aucun crash) | `22` (3 erreurs Memcheck) | En natif, `free(a)` place le bloc dans le `tcache` `glibc` et écrase les 16 premiers octets (`b[0..3]`) avec `tcache_entry` (`next` + `key`). Valgrind met le bloc en quarantaine sans écrire de clé `tcache`, tout en marquant ses octets comme inaccessibles dans la *shadow memory*. |
| **Pas entre blocs tas (`./heap`)** | `+0x20` (**32 octets**) | `+0x50` (**80 octets** : `0x4a91480` → `...4d0` → `...520` → `...570`) | `glibc` utilise une taille minimale de chunk de 32 octets sur 64 bits. Memcheck insère des **redzones** (zones de garde interdites) autour de chaque bloc pour détecter les débordements de tas. |
| **Tampon `stdout` (`1 024` o)** | Invisible (trahi par l'offset initial `...420`) | Visible explicitement (`1,024 bytes`) dans les 4 exécutables | Valgrind confirme au byte près l'allocation du tampon `stdout` de `glibc` et la taille `sizeof(Person) == 16` (`1 024 + 16 + 6 + 16 + 4 = 1 066` o dans `./heap`). |

---

## 4. Audit critique : usage de l'IA, erreurs et omissions corrigées

L'IA a été utilisée comme aide initiale pour catégoriser les diagnostics Memcheck et formuler une première interprétation, puis chaque explication a été vérifiée indépendamment face au code source C et aux cartographies mémoire de la Tâche 0.

| Programme | Erreur ou omission initiale de l'IA | Pourquoi c'était incorrect / incomplet | Correction apportée après vérification |
| :--- | :--- | :--- | :--- |
| **`crash_example.c`** | Interpréter `1,024 bytes still reachable` comme une fuite du programme utilisateur, puis le qualifier de « faux positif ». | Les deux formulations sont inexactes : ce n'est pas une fuite du code C (la trace pointe vers `_IO_file_doallocate` / `puts` ligne 27), et ce n'est pas non plus un faux positif de Valgrind puisque le bloc est réellement encore alloué et atteignable au moment du `SIGSEGV`. | Reclassé précisément en **allocation interne `glibc` toujours atteignable**, interrompue avant le nettoyage `libc` par le `SIGSEGV`, et exclue de `ERROR SUMMARY` par défaut. |
| **`aliasing_example.c`** | Supposer que Valgrind signale la ligne 40 (comme GCC `-Wuse-after-free`) ou que `b[2]` continue d'afficher `-738082945` sous Valgrind. | Contredit par la sortie réelle : Valgrind rapporte exactement **3 erreurs** (lignes 42, 44, 45) et affiche `b[2]=22`. | Distinction explicite entre l'analyse statique de GCC (norme C sur la ligne 40) et la détection dynamique de Memcheck (déréférencements effectifs uniquement), ainsi que le rôle de `vg_replace_malloc.c`. |
| **`crash_example.c` & `heap_example.c`** | Omissions au premier passage : absence d'explication sur `puts` (ligne 27), sur l'écart de 80 octets dans `./heap`, et sur le bilan de la mémoire non initialisée. | Un rapport incomplet laisse des incohérences apparentes entre le code source (`printf`), la Tâche 0 (`32` o) et la sortie Valgrind (`puts`, `80` o). | Ajout de l'optimisation GCC `printf` → `puts`, des *redzones* Valgrind (`0x50` = 80 o) et de la vérification explicite des valeurs non initialisées (`0` erreur). |

---

## 5. Résumé

* **Raisonnement autonome (indépendant de la sortie brute) :** chaque anomalie s'explique d'abord par la violation du modèle mémoire C — transfert de propriété rompu dans `./heap` (`alice` libéré avant `alice->name`), déréférencement d'un alias non propriétaire après `free(a)` dans `./aliasing`, et écriture sur un retour `NULL` non vérifié dans `./crash` — et Valgrind ne fait que confirmer matériellement ces violations.
* **Validation chiffrée au byte près :** les totaux Valgrind (`1 024` o, `1 066` o, `1 044` o) confirment exactement `sizeof(Person) == 16`, la taille de chaque allocation utilisateur et le tampon interne `stdout` de `glibc`.
* **Compréhension des outils :** l'analyse distingue l'allocateur natif `glibc` (`tcache`, chunks de 32 o) de Memcheck (`vg_replace_malloc.c`, quarantaine, *redzones* de 80 o), ainsi que l'analyse statique GCC face au suivi dynamique de validité mémoire.
* **Esprit critique :** traiter l'IA comme une hypothèse et vérifier systématiquement les piles d'appels a corrigé les raccourcis initiaux (notamment sur le bloc `still reachable` de `./crash` et les différences natif/Valgrind sur `./aliasing`).
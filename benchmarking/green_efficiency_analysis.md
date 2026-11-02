1. Méthodologie de mesure

Pour mesurer le temps de CPU que les tâches prennent quand elles s'exécutent, il faut mettre le début du chronomètre avant la ligne de commande de la tâche en question puis le finir après celle-ci.
Afin de s'y retrouver plus facilement, on peut les nommer "start" et "end" en leur assignant le type clock_t (tout le temps valable quel que soit le nom donné) afin de faciliter par la suite le calcul effectif du temps. En effet, nous devons par la suite effectuer le calcul suivant : (double)(end - start) / (double)CLOCKS_PER_SEC où nous demandons de soustraire au temps final (end) le temps initial (start) et le tout diviser par la constante CLOCKS_PER_SEC. Le numérateur permet de connaître la différence entre deux mesures de clock() (en unités d'horloge) et le dénominateur de convertir les unités d'horloge en secondes (unité d'horloge/seconde). Le calcul donne donc un résultat en secondes.


2. Différences de performance observées

Avec les résultats des tâches précédentes, nous pouvons observer deux phénomènes : que pour un même programme lancé plusieurs fois, la durée d'exécution n'est pas la même. Par exemple pour la tâche 0, nous avons 0.184s, 0.192s et 0.190s (arrondis). Nous pouvons observer la même chose pour la première tâche effectuée : 2.588695s, 2.283229s et 2.212223s pour la boucle for imbriquée dans une première boucle et 0.000085s, 0.000082s et 0.000082s pour la boucle for simple. Cela nous permet également de voir qu'une répétition de boucles imbriquées entre elles met plus de temps pour s'effectuer. Pour être plus précis et dans le cas de la tâche numéro 1, pour deux différentes exécutions, nous avons que la durée d'exécution de la boucle for simple est en moyenne 99,9965% plus efficace que la double boucle for soit environ 28 450 fois plus rapide. Il faut cependant noter que le résultat obtenu n'est pas le même selon le type de boucle : 625025000 pour la boucle double et 25000 pour la simple à cause du code écrit.


3. Relation entre le temps d’exécution et la consommation d’énergie

Grâce aux observations précédentes où nous avons pu constater une durée d'exécution presque 100% plus efficace pour une boucle for simple par rapport à une double boucle for, nous pouvons en déduire que si le CPU est moins sollicité sur la durée comme c'est le cas ici, ce dernier consomme moins de puissance de l'ordinateur et donc sans doute moins d'énergie. Nous pouvons donc émettre l'hypothèse que pour un même nombre d'exécutions, plus de boucles il y a, d'autant plus si elles sont imbriquées entre elles plus, plus la consommation électrique est élevée et par conséquent, que notre programme est moins écologique qu'il aurait pu l'être.


4. Limites de l’expérience

Nous avons testé dans les différentes tâches du projet uniquement les boucles for. La supposition présente dans le point numéro 3 est donc à prendre avec des pincettes. Nous ne savons en effet pas, par manque d'expérimentation, ce que donnent les autres boucles while et do while sur le temps d'exécution du CPU. Pour réellement connaître et mesurer la consommation électrique et l'usure des composants engendrée, il faudrait - en plus des données que nous avons - avoir les données en temps réel de la consommation électrique et issues des capteurs présents au niveau des composants de la machine.


5. Enseignements pratiques pour le développement logiciel

Au final, avec toutes ces tâches effectuées, toutes ces expérimentations faites, malgré les manques comme vu précédemment, et avec tous les résultats analysés, nous pouvons dire avec assurance que plus un programme est simple, moins les ressources monopolisées seraient importantes. Par conséquent, une rapidité d'exécution du programme en cours permet ainsi une libération de la place sur la mémoire vive plus rapide.
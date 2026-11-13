#include <stdio.h>
/*calcculatrice*/

int main(void)
{
	/* Tache 0 : présentation de la calculatrice avec un message de bienvenue, le menu et quitter le programme */
	/* Tache 1 : répétition du menu jusqu'au départ de l'utilisateur + refus des choix invalides*/
	int choix;
	int A, B;
	int resultat;

	while (choix != 0)
	{
		/* affichage de la calculatrice */
		printf("Simple Calculator\n");
		printf("1) Add\n");
		printf("2) Subtract\n");
		printf("3) Multiply\n");
		printf("4) Divide\n");
		printf("0) Quit\n");
		/* permet de récuperer la valeur émis par l'utilisateur :
		* je veux ici prendre l'adresse du pointeur choix pour écrire directement
		* la réponse de l'utilisateur dans et non pas réatribuer "choix" */
		scanf("%d", &choix);
		/* affichage du choix de l'utilisateur sur le menu précédent */
		printf("Choice: %d\n", choix);
		if (choix == 0)
			printf("Bye!\n");
		if (choix != 1 ||choix != 2 ||choix != 3 ||choix != 4 ||choix != 0)
			printf("Invalid choice\n");

		/* Tache 2 : ajouter l'addition entre deux nombre A et B. Ces derniers ont été ajouté
		*lors de la déclaration au début de la fonction pour être utilisé sur d'autres opérations*/
		if (choix == 1)
		{
			/* choix de A par l'utilisateur */
			scanf("%d", &A);
			printf("A: %d\n", A);

			/* choix de B par l'utilisateur */
			scanf("%d", &B);
			printf("B: %d\n", B);

			/* Calcul A + B */
			resultat = A + B;

			printf("Result: %d\n", resultat);
		}

		/* Tache 3 : ajouter la soustraction entre deux nombre A et B. Ces derniers ont été ajouté
		* lors de la déclaration au début de la fonction pour être utilisé sur d'autres opérations*/
		if (choix == 2)
		{
			/* choix de A par l'utilisateur */
			scanf("%d", &A);
			printf("A: %d\n", A);

			/* choix de B par l'utilisateur */
			scanf("%d", &B);
			printf("B: %d\n", B);

			/* Calcul A + B */
			resultat = A - B;

			printf("Result: %d\n", resultat);
		}
	}

	return (0);
 }
#include <stdio.h>
/*calcculatrice*/

int main(void)
{
	/* Tache 1 : présentation de la calculatrice avec un message de bienvenue, le menu et quitter le programme */
	float choix;

	/* affichage de la calculatrice */
	printf("Simple Calculator\n");
	printf("1) Add\n");
	printf("2) Subtract\n");
	printf("3) Multiply\n");
	printf("4) Divide\n");
	printf("0) Quit\n");
	/* permet de récuperer la valeur émis par l'utilisateur */
	/* je veux ici prendre l'adresse du pointeur choix pour écrire directement la réponse de l'utilisateur dans et non pas réatribuer "choix" */
	scanf("%lf", &choix);
	/* affichage du choix de l'utilisateur sur le menu précédent */
	printf("Choice: %lf\n", &choix);
	if (choix == 0)
		printf("Bye!\n");

	return (0);
 }
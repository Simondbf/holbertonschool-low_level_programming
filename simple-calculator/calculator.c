#include <stdio.h>
/*calcculatrice*/

int main(void)
{
	/* Tache 1 : présentation de la calculatrice avec un message de bienvenue, le menu et quitter le programme */
	int choix;

	/* permet de récuperer la valeur de l'utilisateur*/
	scanf("%d");

	/* affichage de la calculatrice */
	printf("Simple Calculator\n");
	printf("1) Add\n");
	printf("2) Subtract\n");
	printf("3) Multiply\n");
	printf("4) Divide\n");
	printf("0) Quit\n");
	printf("Choice: %d\n", choix);
	printf("Bye!\n");

	return (0);
 }
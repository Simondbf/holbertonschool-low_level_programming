#include "main.h"
#include <stdio.h>
/**
 * print_array - affiche n éléments d’un tableau d’entiers + retour ligne
 * Return: 0
 * @a: pointeur
 * @n: taille du tableau
 */
void print_array(int *a, int n)
{
	int i = 0;
	int nbr = 0;  /*nombre à imprimer dans le tableau*/

	if (n <= 0)
	{
		printf("\n");
	}
	for (; i < n; i++)
	{
		if (i == n - 1) /*-1 car va trop loin sinon*/
		{
			printf("%d\n", a[nbr]);
		}
		else
		{
		printf("%d, ", a[nbr]);
		}
		nbr++;
	}

}

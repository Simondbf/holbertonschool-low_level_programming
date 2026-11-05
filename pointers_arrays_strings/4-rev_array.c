#include "main.h"
/**
 * reverse_array - inverse l'ordre des élément d'un tableau
 * Return: 0
 * @a: pointeur
 * @n: taille du tableau
 */
void reverse_array(int *a, int n)
{
	int debut = 0, fin = 0, temporaire = 0;

	fin = n - 1;
	for (; debut < fin; debut++, fin--)
	{
		temporaire = a[debut];
		a[debut] = a[fin];
		a[fin] = temporaire;
	}
}

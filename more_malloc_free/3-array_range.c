#include "main.h"
#include <stdlib.h>
/**
 * array_range - créer un tableau d'entier
 * @min: valeur minimum
 * @max: valeur maximum
 * Return: pointeur vers le tableau 2D, ou NULL en cas d'échec
 */
int *array_range(int min, int max)
{
	int taille, i, j;
	int *p;

	if (min > max)
		return (NULL);

	/*plus lisible mais pas obligatoire à mettre*/
	taille = max - min + 1;

	p = malloc(sizeof(int) * taille);
	if (p == NULL)
		return (NULL);

	for (i = min, j = 0; i <= max; i++, j++)
		p[j] = i;

	return (p);
}

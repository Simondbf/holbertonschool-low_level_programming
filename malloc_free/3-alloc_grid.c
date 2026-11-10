#include "main.h"
#include <stdlib.h>
/**
 * alloc_grid - fonction qui retourne un pointeur vers un tableau 2D
 * @width: largeur (colonnes)
 * @height: hauteur (lignes)
 * Return: pointeur vers le tableau 2D, ou NULL en cas d'échec
 */
int **alloc_grid(int width, int height)
{
	int **pp;
	int w, h, i;

	if (width <= 0 || height <= 0)
		return (NULL);

	pp = malloc(sizeof(int *) * height);
	if (pp == NULL)
		return (NULL);

	for (h = 0; h < height; h++)
	{
		pp[h] = malloc(sizeof(int) * width);
		if (pp[h] == NULL)
		{
			for (i = 0; i < h; i++)
				free(pp[i]);
			free(pp);
			return (NULL);
		}
		for (w = 0; w < width; w++)
			pp[h][w] = 0;
	}
	return (pp);
}
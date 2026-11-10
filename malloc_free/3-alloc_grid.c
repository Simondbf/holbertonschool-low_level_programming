#include "main.h"
#include <stdlib.h>
/**
 * alloc_grid - fontion qui retourne pointeur vers tableau 2D
 * @width: largeur (colonnes)
 * @height: hauteur (lignes)
 * Return: pointeur vers tableau 2D
 */
int **alloc_grid(int width, int height)
{
	int **pp;
	int w = 0, h = 0;

	if (width <= 0 || height <= 0)
	{
		return (NULL);
	}
	pp = malloc(sizeof(int *) * height);
	if (pp == NULL)
	{
		return (NULL);
	}
	for (; h <= height; h++)
	{
		pp[h] = malloc(sizeof(int *) * width);
		if (pp[h] == NULL)
		{
			for (; w <= width; w++)
			{
				free(pp[w]);
			}
			free(pp);
			return (NULL);
		}
		for (w = 0; w <= width; w++)
		{
			pp[h][w] = 0;
		}
	}
	return (pp);
}

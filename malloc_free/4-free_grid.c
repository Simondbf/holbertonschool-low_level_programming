#include "main.h"
#include <stdlib.h>
/**
 * free_grid - libération mémoire de la grille 2D de la tache 3
 * @grid: double pointeur
 * @height: hauteur (lignes)
 * Return: pointeur vers le tableau 2D, ou NULL en cas d'échec
 */
void free_grid(int **grid, int height)
{
	int h;

	for (h = 0; h < height - 1; h++)
	{
		free(grid);
	}
}

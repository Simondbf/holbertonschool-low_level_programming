#include "main.h"
#include <stdlib.h>
/**
 * malloc_checked - libération mémoire de la grille 2D de la tache 3
 * @b: entier non signé
 * Return: pointeur vers le tableau 2D, ou NULL en cas d'échec
 */
void *malloc_checked(unsigned int b)
{
	void *p;

	p = malloc(b);
	if (p == NULL)
		exit(98);
	return (p);
}

#include "main.h"
#include <stdlib.h>
/**
 * _calloc - alloue de la mémoire pour un tableau, return pointeur
 * @nmemb: nbre d'élément dans le tableau
 * @size: nobres d'octet
 * Return: pointeur vers le tableau 2D, ou NULL en cas d'échec
 */
void *_calloc(unsigned int nmemb, unsigned int size)
{
	unsigned int *p, i, j, taille;

	if (size == 0 || nmemb == 0)
		return (NULL);
	/*taille du tableau*/
	taille = size * nmemb;

	/*malloc + return obligatoire du malloc*/
	p = malloc(taille);
	if (p == NULL)
		return (NULL);

	/*allocation chaque octet à 0, en gros chaque case du tableau*/
	for (; i < taille; i++)
	{
		for (; j < taille; j++)
			p[i] = 0;
	}

	return (p);
}

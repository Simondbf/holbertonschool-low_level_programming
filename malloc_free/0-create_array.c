#include "main.h"
#include <stdlib.h>
/**
 * *create_arrays - crée un tableau de caractère
 * @c: caractère initialisateur du tableau
 * @size: nombre de caractère à reserver
 * Return: pointeur vers le tableau ou null si echec
 */
char *create_array(unsigned int size, char c)
{
	char *p;
	unsigned int i;

	if (size == NULL)
	{
		return (NULL);
	}

	p = malloc(sizeof(char) * size);
		if (p == NULL)
	{
		return (NULL);
	}

	for (i = 0; i < size; i++)
	{
		p[i] = c;
	}

	return (p);
}

#include "main.h"
#include <stdlib.h>
/**
 * *create_arrays - crée un tableau + initialise caractère spécifique
 * @c: caractère
 * @size: entier
 * Return:
 */
char *create_array(unsigned int size, char c)
{
	char *p;
	unsigned int i;

	p = malloc(sizeof(int) * size);

	if (p == NULL)
	{
		return ('\0');
	}
	for (i = 0; i <= size; i++)
	{
		p[i] = c;
	}

	return (p);
}

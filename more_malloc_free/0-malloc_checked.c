#include "main.h"
#include <stdlib.h>
/**
 * malloc_checked - alloue de la mémoire avec malloc et quitte si ça échoue
 * @b: nombre d'octets à allouer
 * Return: pointeur vers la mémoire allouée
 */
void *malloc_checked(unsigned int b)
{
	void *p;

	p = malloc(b);
	if (p == NULL)
		exit(98);
	return (p);
}

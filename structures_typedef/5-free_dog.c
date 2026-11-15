#include "dog.h"
#include <stdio.h>
#include <stdlib.h>
/**
 * free_dog - libération de l'espace mémoire de la structure
 * @d: pointeur de la structure via l'alias dog_t
 * Return: vide car void en cas d'espace mémoire non alloué
 */
void free_dog(dog_t *d)
{
	if (d == NULL)
		return;

	free(d->name);
	free(d->owner);
	free(d);
}

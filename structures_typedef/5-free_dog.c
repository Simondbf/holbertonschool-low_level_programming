#include "dog.h"
#include <stdio.h>
#include <stdlib.h>
/**
 * _strcpy - fonction de copie dans la zone mémoire
 * @dest: zone de destination à copier
 * Return: dest_copy, zone de mémoire copier
 */
void free_dog(dog_t *d)
{
	free(d);
}
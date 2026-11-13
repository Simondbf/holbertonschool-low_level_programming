#include "dog.h"
#include <stdio.h>
/**
 * init_dog - fonction initialise la variable dela strcture
 * @d: pointeur de la structure
 * @name: nom du chien
 * @age: age du chien
 * @owner: proprio
 */
void init_dog(struct dog *d, char *name, float age, char *owner)
{
	/*voir sur une valeur manque*/
	if (d == NULL)
		return;

	d->name = name;
	d->age = age;
	d->owner = owner;
}

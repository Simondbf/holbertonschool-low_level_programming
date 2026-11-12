#include "dog.h"
/**
 * init_dog - fonction initialise la variable dela strcture
 * struct dog - structure
 * @d: pointeur de la structure
 * @name: nom du chien
 * @age: age du chien
 * @owner: proprio
 */
void init_dog(struct dog *d, char *name, float age, char *owner)
{
	d->name = "Poppy";
	d->age = 3.5;
	d->owner = "Bob";
}

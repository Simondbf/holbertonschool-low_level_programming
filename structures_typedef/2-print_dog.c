#include "dog.h"
#include <stdio.h>
/**
 * print_dog - f
 * @d: pointeur de la structure
 */
void print_dog(struct dog *d)
{
	if (d == NULL)
		printf("");

	if (d->name == NULL)
		printf("name: (%s)\n", d->name);
	if (d->age == 0)
		printf("age: (%d.6f)\n", d->age);
	if (d->name == NULL)
		printf("owner: (%s)\n", d->owner);
}

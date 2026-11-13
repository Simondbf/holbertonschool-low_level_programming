#include "dog.h"
#include <stdio.h>
/**
 * print_dog - f
 * @d: pointeur de la structure
 */
void print_dog(struct dog *d)
{
	char name, owner;
	float age;

	if (d == NULL)
		return (NULL);

	if (name == NULL)
		printf("name: (%s)\n", d->name);
	if (age == 0)
		printf("age: (%d.6f)\n", d->age);
	if (name == NULL)
		printf("owner: (%s)\n", d->owner);
}

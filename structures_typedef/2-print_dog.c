#include "dog.h"
#include <stdio.h>
#include <stdlib.h>
/**
 * print_dog - f
 * @d: pointeur de la structure
 */
void print_dog(struct dog *d)
{
	/*précaution*/
	if (d == NULL)
		exit;

	/*conditions spécifiques de la consigne*/
	if (d->name == NULL)
		printf("name: (nil)\n");
	else
		printf("name: (%s)\n", d->name);

	printf("age: (%.6f)\n", d->age);

	if (d->name == NULL)
		printf("owner: (nil)\n");
	else
		printf("owner: (%s)\n", d->owner);
}

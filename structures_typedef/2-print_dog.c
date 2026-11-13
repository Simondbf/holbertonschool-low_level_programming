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
		return;

	/*conditions spécifiques de la consigne*/
	if (d->name == NULL)
		printf("Name: (nil)\n");
	else
		printf("Name: %s\n", d->name);

	printf("Age: %.6f\n", d->age);

	if (d->owner == NULL)
		printf("Owner: (nil)\n");
	else
		printf("Owner: %s\n", d->owner);
}

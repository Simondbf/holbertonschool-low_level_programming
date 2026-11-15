#include "dog.h"
#include <stdio.h>
#include <stdlib.h>
/**
 * new_dog - nouveau chien
 * @name: nom
 * @age: age
 * @owner: proprio du chien
 * Return: NULL en cas d'échec
 */


char *_strcpy(char *dest)
{
	int i = 0, size = 0;
	char *dest_copy;

	if (dest == NULL)
		return (NULL);
	for (; dest[size] != '\0'; size++)
	{	}
	size++; /*taille +1 pour ajouter le \0*/
	dest_copy = malloc(sizeof(char) * size);
	if (dest_copy == NULL)
		return (NULL);
	for (; i < size; i++)
		dest_copy[i] = dest[i];
	return (dest_copy);
}


dog_t *new_dog(char *name, float age, char *owner)
{
	dog_t *dog2;

	if (name == NULL || owner == NULL)
		return (NULL);

	dog2 = malloc(sizeof(dog_t));
	if (dog2 == NULL)
		return (NULL);

	dog2->name = _strcpy(name);
	if (dog2->name == NULL)
	{
		free(dog2);
		return (NULL);
	}

	dog2->owner = _strcpy(owner);
	if (dog2->owner == NULL)
	{
		free(dog2->name);
		free(dog2);
		return (NULL);
	}

	dog2->age = age;

	return (dog2);
}

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
dog_t *new_dog(char *name, float age, char *owner)
{
	name = malloc(sizeof(char));
	if (name == NULL)
		return (NULL);

	owner = malloc(sizeof(char));
	if (owner == NULL)
		return (NULL);



	free(name);
	free(owner);


	return ();
}

 * _strcpy - copie de chaine de caractère pointé par scr vers le buffer(zone mémoire) pointé par dest
char *_strcpy(char *dest, char *src)
{
	int i = 0;

	for (; src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
	dest[i] = '\0';
	return (dest);
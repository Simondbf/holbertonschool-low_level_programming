#include "main.h"
#include <stdlib.h>
/**
 * _strdup - return pointeur → new memory zone contenant chaine de caractère
 * @str: pointeur
 * Return: pointeur vers chaine duppliquée, ou NULL en cas d'échec
 */
char *_strdup(char *str)
{
	char *p;
	int size = 0;
	int i = 0;

	if (str == 0)
	{
		return (NULL);
	}
	for (; str[i] != '\0'; i++)
	{
		size++;
	}
	size++;							/*pour inclure null byte*/
	p = malloc(sizeof(char) * size);
	if (p == NULL)
	{
		return (NULL);
	}
	for (i = 0; i <= size; i++)
	{
		p[i] = str[i];
	}
	return (p);
}

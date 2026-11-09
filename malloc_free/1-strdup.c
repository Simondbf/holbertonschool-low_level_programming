#include "main.h"
#include <stdlib.h>
/**
 * _strdup - return pointeur → new memory zone contenant chaine de caractère
 * @str: pointeur
 * Return: pointeur vers chaine duppliquée, ou NULL en cas d'échec
 */
char *_strdup(char *str)
{
	char *p, size = 0;
	int i = 0, j = 0;

	if (str == 0)
	{
		return (NULL);
	}
	for (; str[i] != '\0'; i++)
	{
		size++;
	}
	str = malloc(sizeof(char) * size);
	p = malloc(sizeof(char) * size);
	if (str == NULL)
	{
		return (NULL);
	}
	for (i = 0; size <= '\0'; i++, j++)
	{
		p[j] = str[i];
	}
	return (str);
	free(str);
}

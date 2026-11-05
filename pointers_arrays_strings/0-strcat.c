#include "main.h"
/**
 * _strcat - coller deux chaînes de caractères.
 * Return: 0
 * @dest: pointeur
 * @src: pointeur
 */
char *_strcat(char *dest, char *src)
{
	int i = 0, j = 0;

	for (; dest[i] != '\0'; i++)
	{	}
	if (dest[i] == '\0')
	{
	}
	for (; src[j] != '\0'; i++, j++)
	{
		dest[i] = src[j];
	}
	dest[i] = '\0';
	return (dest);
}

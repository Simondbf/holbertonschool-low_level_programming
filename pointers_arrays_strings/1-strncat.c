#include "main.h"
/**
 * *_strncat - coller deux chaînes de caractères.
 * Return: 0
 * @dest: pointeur
 * @src: pointeur
 * @int: nombre max d'octet de src
 */
char *_strncat(char *dest, char *src, int n)
{
	int i = 0, j = 0;

	for (; dest[i] != '\0'; i++)
	{	}
	for (; src[j] != '\0'; i++, j++)
	{
		if (j >= n)
		{
			dest[i] = '\0';
			return (dest);
		}
		dest[i] = src[j];
	}
	dest[i] = '\0';
	return (dest);
}

#include "main.h"
/**
 * *_strncpy - copie une chaine de caractère.
 * Return: 0
 * @dest: pointeur
 * @src: pointeur
 * @n: nombre max d'octet de src
 */
char *_strncpy(char *dest, char *src, int n)
{
	int i = 0, j = 0;

	for (; i < n; i++, j++)
	{
		if (src[j] == '\0')
		{
			break;
		}
		dest[i] = src[j];
	}
	for (; i < n; i++)
	{
		dest[i] = '\0';
	}
	return (dest);
}

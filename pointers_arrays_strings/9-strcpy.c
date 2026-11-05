#include "main.h"
/**
 * _strcpy - copie de chaine de caractère pointé par scr + \0
 * vers le buffer(zone mémoire) pointé par dest
 * Return: 0
 * @dest: pointeur
 * @src: pointeur
 */
char *_strcpy(char *dest, char *src)
{
	int i = 0;

	for (; src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
	dest[i] = '\0';
	return (dest);
}

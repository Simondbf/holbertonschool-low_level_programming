#include "main.h"
/**
 * *_memset - 
 * Return: 0
 * @s: pointeur
 * @b: valeur constante
 * @n: n octet de la zone mémoire pointée par s
 */
char *_memset(char *s, char b, unsigned int n)
{
	unsigned int i = 0;

	for (; i < n; i++)
	{
		s[i] = b;
	}
	return (s);
}

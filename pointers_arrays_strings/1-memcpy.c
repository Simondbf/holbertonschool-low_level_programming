#include "main.h"
/**
 * *_memcpy - copie n octet de zone mémoire src vers zone mémoire dest
 * Return: 0
 * @src: zone mémoire initiale
 * @dest: zone mémoire finale
 * @n: n octet de la zone mémoire pointée par s
 */
char *_memcpy(char *dest, char *src, unsigned int n)
{
	unsigned int i = 0;

	for (; i < n; i++)
	{
		dest[i] = src[i];
	}
	return (dest);
}

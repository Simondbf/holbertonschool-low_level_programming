#include "main.h"
/**
 * _strstr - recherche sous chaine de caractère dans une autre chaine
 * Return: 0
 * @haystack: pointeur
 * @needle: pointeur
 */
char *_strstr(char *haystack, char *needle)
{
	int i = 0, j = 0;

	for (; haystack[i] != '\0'; i++)
	{
		for (j = 0; needle[j] != '\0'; j++)
		{
			if (haystack[i] == needle[j])
			{
				return (haystack[i]);
			}
		}
	}
	return (0);
}

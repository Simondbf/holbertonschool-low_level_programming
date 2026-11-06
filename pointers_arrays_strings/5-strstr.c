#include "main.h"
/**
 * _strstr - recherche sous chaine de caractère dans une autre chaine
 * Return: 0
 * @haystack: pointeur
 * @needle: pointeur
 */
char *_strstr(char *haystack, char *needle)
{
	int i = 0;
	int j;

	if (needle[i] == '\0')
	{
		return (&haystack);
	}
	for (i = 0; haystack[i] != '\0'; i++)
	{
		j = 0;
		if (haystack[i] == needle[j])
		{
			for (j = 0; needle[j] != '\0'; j++)
			{
				if (haystack[i] == needle[j])
				{
					if (needle[j + 1] == '\0')
					{return (&haystack[i - j])}
					i++;
				}
			}
		}
	}
	return (0);
}

/*W W O R L D \0*/

#include "main.h"
/**
 * _strchr - recherche caractère dans une chaine
 * Return: 0
 * @s: pointeur
 * @c: caractère recherché
 */
char *_strchr(char *s, char c)
{
	int i = 0;

	for (;; i++)
	{
		if (s[i] == c)
		{
			return (&s[i]);
		}
		if (s[i] == '\0')
		{
			return (0);
		}
	}
}

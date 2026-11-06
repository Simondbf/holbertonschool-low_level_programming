#include "main.h"
/**
 * *leet - encodage selon leet (1337)
 * Return: 0
 * @s: pointeur
 */
char *leet(char *s)
{
	char *start = s;
	char lettres[] = "aAeEoOtTlL";
	char nombres[] = "4433007711";

	char *l; /*le doigt*/
	char *n;

	while (*s != '\0')
	{
		l = lettres; /*mets le doigt sur la 1ere case du tabeau lettres*/
		n = nombres;

		while (*l != '\0')
		{
			if (*s == *l)
			{
				*s = *n;
			}
			l++;
			n++;
		}
		s++;
	}

	return (start);
}

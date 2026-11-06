#include "main.h"
/**
 * _strpbrk - comparaison d'une chaine par rapport à des caractères précis
 * Return: 0
 * @s: pointeur
 * @accept: pointeur
 */
char *_strpbrk(char *s, char *accept)
{
	int i = 0, j = 0;

	for (; s[i] != '\0'; i++)
	{
		for (j = 0; accept[j] != '\0'; j++)
		{
			if (s[i] == accept[j])
			{
				return (&s[i]);
			}
			if (s[i] == '\0')
			{
				return (0);
			}
		}
	}
}

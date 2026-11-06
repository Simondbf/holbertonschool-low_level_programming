#include "main.h"
/**
 * _strspn - comparaison d'une chaine par rapport à des caractères précis
 * Return: 0
 * @s: pointeur
 * @accept: pointeur
 */
unsigned int _strspn(char *s, char *accept)
{
	int i = 0, j = 0;

	for (; s[i] != '\0'; i++)
	{
		for (j = 0; accept[j] != '\0'; j++)
		{
			if (s[i] == accept[j])
			{
				break;
			}
		}
		if (accept[j] == '\0')
		{
			return (i);
		}
	}
	return (i);
}
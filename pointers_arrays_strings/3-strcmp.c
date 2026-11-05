#include "main.h"
/**
 * *_strcmp - compare 2 chaines de caractères.
 * Return: 0
 * @s1: pointeur 1ere chaine
 * @s2: pointeur 2e
 */
int _strcmp(char *s1, char *s2)
{
	int i = 0, j = 0;

	for (; s1[i] - s2[j] == 0 && s1[i] != '\0'; i++, j++)
	{
		if (s1['\0'] == s2['\0'])
		{
			break;
		}
	}
	return (s1[1] - s2[j]);
}

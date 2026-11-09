#include "main.h"
#include <stdlib.h>
/**
 * str_concat - concatène (coller) 2 chaines de cractères
 * @s1: chaine de caractère
 * @s2: idem
 * Return: pointer vers la nouvelle chaine
 */
char *str_concat(char *s1, char *s2)
{
	char *s3;
	int i = 0, j = 0, k = 0;

	if (s1 == NULL)
		s1 = "";

	if (s2 == NULL)
		s2 = "";

	for (; s1[i] != '\0'; i++)
	{ }
	for (; s2[j] != '\0'; j++)
	{ }

	s3 = malloc(sizeof(char) * (i + j));
	for (i = 0; s1[i] != '\0'; i++, k++)
	{
		s3[k] = s1[i];		/* ou *(s3 + i) = *(s1 + j);*/
	}
	for (j = 0; s2[j] != '\0'; j++, k++)
	{
		s3[k] = s2[j];
	}
	s3[k] = '\0';

	return (s3);
}

#include "main.h"
#include <stdlib.h>
/**
 * string_nconcat - concatère 2 chaines ensembles
 * (n char si n < que la 2e chaine) puis \0 après
 * @s1: pointeur de caractères
 * @s2: pointeur de caractères
 * @n: entier non signé
 * Return: pointeur vers le tableau 2D, ou NULL en cas d'échec
 */
char *string_nconcat(char *s1, char *s2, unsigned int n)
{
	char *s3;
	unsigned int i = 0, j = 0, k = 0;

	if (s1 == NULL)
		s1 = "";
	if (s2 == NULL)
		s2 = "";

	/*taille de s3 + \0*/
	for (; s1[i] != '\0'; i++)
	{	}
	for (; s2[j] != '\0'; j++)
	{	}

	if (n < j)
		j = n;

	/*malloc avec la bonne taille: k puis conséquence si NULL*/
	s3 = malloc(i + j + 1);
	if (s3 == NULL)
		return (NULL);

	/*coller les 2 chaines sur la new zone mémoire*/
	for (k = 0; k < i; k++)
		s3[k] = s1[k];
	for (k = 0; k < j; k++)
		s3[i + k] = s2[k];

	s3[i + j] = '\0';
	return (s3);
}

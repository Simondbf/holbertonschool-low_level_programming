#include "main.h"
/**
 * print_rev - inverse une chaine de caractère
 * Return: 0
 * @s: pointeur
 */
void rev_string(char *s)
{
	int longueur = 0, i = 0;
	char temporaire;

	while (s[longueur] != '\0')
	{
		longueur++;
	}
	while (i < longueur / 2)
	{
		temporaire = s[i];
		s[i] = s[longueur - 1 - i];
		s[longueur - 1 - i] = temporaire;
		i++;
	}
}

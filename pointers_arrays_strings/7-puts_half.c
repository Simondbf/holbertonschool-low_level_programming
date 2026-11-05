#include "main.h"
/**
 * puts_half - affiche la moitié d'une chaine de caractère
 * Return: 0
 * @str: pointeur
 */
void puts_half(char *str)
{
	int longueur = 0, i = 0;

	while (longueur != '\0')
	{
		longueur++;
	}
	if (longueur % 2 != 0)
	{
		i = (longueur + 1) / 2;
	}
	else
	{
		i = (longueur) / 2;
	}

	while (i != '\0')
	{
		_putchar(str);
	}
	_putchar('\n');
}

#include "main.h"
/**
 * print_rev - affiche chaine de caractère à l'envers + retour à la ligne
 * Return: 0
 * @s: pointeur
 */
void print_rev(char *s)
{
	while (*s != '\0')
	{
		s++;
	}
	s--;
	for (; *s != '\0'; s--)
	{
		_putchar(*s);
	}
	_putchar('\n');

}

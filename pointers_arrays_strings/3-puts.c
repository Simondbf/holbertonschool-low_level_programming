#include "main.h"
/**
 * _puts - affiche chaine de caractère + retour à la ligne
 * Return: 0
 * @str: pointeur
 */
void _puts(char *str)
{
	for (; *str != '\0'; (*str)++)
	{
		_putchar(*str);
	}
	_putchar('\n');
}

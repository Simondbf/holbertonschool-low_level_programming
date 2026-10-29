#include "main.h"
/**
 * print_sign - affiche le signe d'un nombre
 * @n: nombre à vérifier
 * Return: 0
 */
int print_sign(int n)
{
	if (n > 0)
	{
		return (1);
		_putchar('+');
	}
	else if(n = 0)
	{
		return (0);
		_putchar('0');
	}
	else
	{
		return (-1);
		_putchar('-');
	}
	_putchar('\n');
}

#include "main.h"
/**
 *  print_last_digit - dernier chiffre d'un nombre
 * @n: nombre à vérifier
 * Return: 0
 */
int print_last_digit(int n)
{
	int c;

	if (n > 10)
	{
		c = n % '10';
		_putchar (n);
		return (c);
	}
	else
	{
		_putchar (n);
		return (n);
	}
}

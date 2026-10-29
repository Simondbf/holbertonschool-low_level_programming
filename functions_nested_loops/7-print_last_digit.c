#include "main.h"
/**
 *  print_last_digit - dernier chiffre d'un nombre
 * @n: nombre à vérifier
 * Return: 0
 */
int print_last_digit(int n)
{
	int c;

	c = n % 10;
	if (c < 0)
	{
		c = -c;
	}
	_putchar ('0' + c);
	return (c);
}

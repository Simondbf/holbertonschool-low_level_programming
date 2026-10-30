#include "main.h"
/**
 * print_to_98 - affiche tous les nombres entiers jusqu'à 98 + retour ligne
 * Return: 0
 * @n: nombres entiers
*/
void print_to_98(int n)
{
	int a = 98;

	for (; n < a; n++)
	{
		if (n < 0)
		{
			_putchar('-');
		}
		if (n > 9)
		{
			_putchar('0' + n / 10);
		}
		_putchar('0' + n % 10);
		_putchar(',');
		_putchar(' ');
	}
	for (; n > a; n--)
	{
		if (n > (a + 1))
		{
			_putchar('0' + n / 100);
		}
		_putchar('0' + n / 10);
		_putchar('0' + n % 10);
		_putchar(',');
		_putchar(' ');
	}
	_putchar('0' + n / 10);
	_putchar('0' + n % 10);
	_putchar('\n');
}

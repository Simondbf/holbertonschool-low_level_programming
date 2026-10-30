#include "main.h"
/**
 * print_numbers - affiche les nombres  de 0 à 9
 * Return: 0
*/
void print_numbers(void)
{
	int i;

	for (i = 0; i <= 9; i++)
	{
		_putchar('0' + i);
	}
	_putchar('\n');
}

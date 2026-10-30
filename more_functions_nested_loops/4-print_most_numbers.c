#include "main.h"
/**
 * print_most_numbers - affiche de 0 à 9 sauf 2 et 4 + retour ligne
 * Return: 0
*/
void print_most_numbers(void)
{
	int i;

	for (i = 0; i <= 9; i++)
	{
		if (i != 2 && i != 4)
		{
			_putchar('0' + i);
		}
	}
	_putchar('\n');
}

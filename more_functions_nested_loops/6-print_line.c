#include "main.h"
/**
 * print_line - dessine une ligne droite
 * Return: 0
 * @n: nombre max de _
*/
void print_line(int n)
{
	if (n > 0)
	{
		int i;

		for (i = 0; i <= n; i++)
		{
			_putchar('_');
		}
	}
	_putchar('\n');
}

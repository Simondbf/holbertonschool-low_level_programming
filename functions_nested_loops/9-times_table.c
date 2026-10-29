#include "main.h"
/**
 * times_table - table de multiplication de 9
 * Return: 0
 */
void times_table(void)
{
	int i;
	int n = 9 * i;

	_putchar ('0');
	for (i = 0; i < 9; i++)
	{
		_putchar (',');
		_putchar (' ');
		if (n >= 10)
		{
			int n2 = n / 10;
			int n1 = n % 10;

			_putchar('o' + n2);
			_putchar('0' + n1);
		}
		else
		{
			_putchar('0' + n);
		}
	}
	_putchar('0' + n);
}

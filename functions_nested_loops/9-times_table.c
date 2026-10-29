#include "main.h"
/**
 * times_table - table de multiplication de 9
 * Return: 0
 */
void times_table(void)
{
	int i;
	int j;
	int n;

	for (i = 0; i <= 9; i++)
	{
		for (j = 0; j <= 9; j++)
		{
			n = i * j;
			if (n <= 9)
			{
				if (j > 1)
				{
					_putchar(' ');
				}
				_putchar('0' + n);
			}
			else
			{
				_putchar('0' + n / 10);
				_putchar('0' + n % 10);
			}
			if (j < 9)
			{
				_putchar(',');
				_putchar(' ');
			}
		}
		_putchar('\n');
	}
}

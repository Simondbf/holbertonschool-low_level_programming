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
			if (j < 9)
			{
				_putchar('0' + n);
				_putchar(',');
				_putchar(' ');
			}
			else
			{
				_putchar('0' + n);
			}
		}
		_putchar('\n');
	}
}

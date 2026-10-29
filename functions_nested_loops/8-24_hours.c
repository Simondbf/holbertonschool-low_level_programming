#include "main.h"
/**
 *  jack_bauer - de minuit à 23:59
 * Return: 0
 */
void jack_bauer(void)
{
	int h;
	int min;

	for (h = 0; h <= 23; h++)
	{
		for (min = 0; min <= 59; min++)
		{
			if (h < 10)
			{
				_putchar('0');
				_putchar('0' + h);
			}
			else
			{
				int h2 = h / 10;
				int h1 = h % 10;

				_putchar('0' + h2);
				_putchar('0' + h1);
			}
			_putchar(':');
			if (min < 10)
			{
				_putchar('0');
				_putchar('0' + min);
			}
			else
			{
				int min2 = min / 10;
				int min1 = min % 10;

				_putchar('0' + min2);
				_putchar('0' + min1);
			}
			_putchar('\n');
		}
	}
}

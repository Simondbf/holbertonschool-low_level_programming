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
				_putchar('0' + (h / 10));
				_putchar('0' + (h % 10));
			}
			_putchar(':');
			if (min < 10)
			{
				_putchar('0');
				_putchar('0' + min);
			}
			else
			{
				_putchar('0' + (min / 10));
				_putchar('0' + (min % 10));
			}
			_putchar('\n');
		}
	}
}

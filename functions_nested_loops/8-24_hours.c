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
		if (h < 10)
		{
			_putchar('0');
		}
		_putchar('0' + h);
		_putchar(':');
		for (min = 0; min <= 59; min++)
		{
			if (min < 10)
			{
				_putchar('0');
			}
			_putchar('0' + min);
		}
	}
}

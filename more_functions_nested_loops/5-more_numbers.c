#include "main.h"
/**
 * more_numbers - affiche 0 à 14 10x
 * Return: 0
*/
void more_numbers(void)
{
	int i;
	int rep;

	for (rep = 1; rep <= 10; rep++)
	{
		for (i = 0; i <= 14 ; i++)
		{
			if (i > 9)
			{
				_putchar('0' + i / 10);
			}
			_putchar('0' + i % 10);
		}
		_putchar('\n');
	}
}

#include "main.h"
/**
 * puts_half - affiche la moitié d'une chaine de caractère
 * Return: 0
 * @str: pointeur
 */
void puts_half(char *str);
{



	_putchar('\n');
}



{
	int i = 0;

	for (; *str != '\0'; str++, i++)
	{
		if (i % 2 == 0)
		{
		_putchar(*str);
		}
	}
	_putchar('\n');
}

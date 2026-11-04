#include "main.h"
/**
 * puts2 - affiche 1 caractère/2 d'une chaîne de caractères à partir du 1er
 * Return: 0
 * @str: pointeur
 */
void puts2(char *str)
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

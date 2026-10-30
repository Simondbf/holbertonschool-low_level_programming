#include "main.h"
/**
 * print_square - dessine un carré
 * Return: 0
 * @size: nombre max de #
*/
void print_square(int size)
{
	int colonne, ligne;

	if (size <= 0)
	{
		_putchar('\n');
	}
	for (colonne = 0; colonne < size; colonne++)
	{
		for (ligne = 0; ligne < size; ligne++)
		{
			_putchar('#');
		}
		_putchar('\n');
	}
}

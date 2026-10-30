#include "main.h"

/**
 * print_triangle - dessiner un triangle
 * Return: 0
 *  @size: taille
*/
void print_triangle(int size)
{
	int colonne, ligne;

	if (size <= 0)
	{
		_putchar('\n');
	}
	else
	{
		for (ligne = 1; ligne <= size; ligne++)
		{
			for (colonne = 1; colonne <= size; colonne++)
			{
				if (colonne <= (size - ligne))
				{
					_putchar(' ');
				}
				else
				{
					_putchar('#');
				}
			}
			_putchar('\n');
		}
	}
}

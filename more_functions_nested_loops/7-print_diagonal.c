#include "main.h"
/**
 * print_diagonal - dessine une diagonale
 * Return: 0
 * @n: nombre max de _
*/
void print_diagonal(int n)
{
	int colonne;
	int ligne;

	if (n > 0)
	{
		for (colonne = 0; colonne < n; colonne++)
		{
			for (ligne = 0; ligne < colonne; ligne++)
			{
				_putchar(' ');
			}
			_putchar(92);
			_putchar('\n');
		}
	}
	else
	{
		_putchar('\n');
	}

}

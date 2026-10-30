#include "main.h"
/**
 * print_triangle - dessiner un triangle
 * Return: 0
 *  @size: taille
*/

/**
 * main - check the code
 *
 * Return: Always 0.
 */
int main(void)
{
    print_triangle(2);
    print_triangle(10);
    print_triangle(1);
    print_triangle(0);
    return (0);
}

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

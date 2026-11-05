#include "main.h"
/**
 * print_array - affiche n éléments d’un tableau d’entiers + retour ligne
 * Return: 0
 * @a: pointeur
 * @n: nombre d'élément à mettre dans le tableau
 */
void print_array(int *a, int n)
{
	int longueur = 0, i = 0;

	while (a[longueur] != '\0')
	{
		longueur++;
	}
	while (a[i] != '\0')
	{
		_putchar(a[i]);
		i++;
	}
	_putchar('\n');
}

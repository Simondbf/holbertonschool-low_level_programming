#include "main.h"
#include <stdio.h>
/**
 * print_diagsums - affiche la somme des deux diagonales
 * @a: pointeur vers la matrice
 * @size: nombre de lignes et de colonnes
 * Return: rien
 */
void print_diagsums(int *a, int size)
{
	int i = 0;
	int diag1 = 0, diag2 = 0;

	for (; i < size; i++)
	{
		diag1 += a[i * size + i];
	}

	for (i = size - 1; i >= 0; i--)
	{
		diag2 += a[i * size + (size - 1 - i)];
	}

	printf("%d, %d\n", diag1, diag2);
}

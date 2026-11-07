#include "main.h"
#include <stdio.h>
/**
 * print_diagsums - affiche la somme des éléments de
 * 2 diagonales d'une matrice carrée
 * Return: 0
 * @a: pointeur
 * @size: taille de la matrice
 */
void print_diagsums(int *a, int size)
{
	int i = 0;
	int diag1 = 0, diag2 = 0;

	for (; i <= size ; i++)
	{
		diag1 = diag1 + a[i * size + i];
	}
	for (i = size - 1; i >= 0; i--)
	{
		diag2 = diag2 + a[i * size - i];
	}
	printf("%d, %d\n", diag1, diag2);
}

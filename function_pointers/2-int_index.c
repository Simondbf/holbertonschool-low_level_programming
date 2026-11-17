#include "function_pointers.h"
#include <stddef.h>

/**
* int_index - recherche un entier
* @array: pointeur
* @size: taille du tableau array
* @cmp: pointeur vers la fonction à utiliser pour comparer les valeurs
* Return: -1 ou le 1er élément où il n'y a pas de return 0
*/
int int_index(int *array, int size, int (*cmp)(int))
{
	int i = 0;

	if (size <= 0)
		return (-1);

	for (; i < size - 1; i++)
	{
		cmp(array[i]);
		if (cmp(array[i]) != 0)
			break;
	}
	if (cmp(array[i]) == 0)
		return (-1);
	return (i);
}

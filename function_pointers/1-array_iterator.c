#include "function_pointers.h"
#include <stddef.h>

/**
* array_iterator - fonction passée en paramètre sur les éléments du tableau
* @array: pointeur
* @size: taille du tableau
* @action: pointeur vers la fonction à utiliser
* Return - rien car void
*/
void array_iterator(int *array, size_t size, void (*action)(int))
{
	size_t i = 0;

	if (array == NULL || action == NULL)
		return;

	for (; i < size; i++)
		action(array[i]);
}

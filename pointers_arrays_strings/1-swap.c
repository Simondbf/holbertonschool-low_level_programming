#include "main.h"
/**
 * swap_int - pointeur vers un entier
 * Return: 0
 * @a: a
 * @b: b
 */
void swap_int(int *a, int *b)
{
	int c;

	c = *a;
	*a = *b;
	*b = c;
}

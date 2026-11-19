#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "variadic_functions.h"

/**
 * sum_them_all - additionne tous les parametre passés
 * @n: constante <= 0
 * Return: somme de tous les parametres
 */
int sum_them_all(const unsigned int n, ...)
{
	va_list nbre; /*outils de lecture de la suite - marque page en gros*/
	int addition = 0;
	unsigned int i;

	va_start(nbre, n); /*indication du début - mise en place du marque page*/

	if (n == 0)
		return (0);

	for (i = 0; i < n; i++)
		addition += va_arg(nbre, int);

	va_end(nbre);
	return (addition);
}

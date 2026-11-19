#include <stdio.h>
#include <stdarg.h>
#include "variadic_functions.h"

/**
 * print_numbers - affiche les nombres
 * @separator: chaine de caractère de séparation entre les nombres
 * @n: constante <= 0
 * Return: somme de tous les parametres
 */
void print_numbers(const char *separator, const unsigned int n, ...)
{
	va_list nbre;
	unsigned int i;

	va_start(nbre, n);

	for (i = 0; i < n; i++)
	{
		if (separator == 0 || i == n - 1)
			printf("%d", va_arg(nbre, int));
		else
			printf("%d%s", va_arg(nbre, int), separator);
	}
	putchar('\n');
	va_end(nbre);

}

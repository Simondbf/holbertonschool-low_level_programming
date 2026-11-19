#include <stdio.h>
#include <stdarg.h>
#include "variadic_functions.h"

/**
 * print_numbers - affiche de chaines de caractères
 * @separator: chaine de caractère de séparation entre les nombres
 * @n: constante <= 0
 * Return: somme de tous les parametres
 */
void print_strings(const char *separator, const unsigned int n, ...)
{
	va_list chains;
	unsigned int i;
	char *cool;

	va_start(chains, n);

	for (i = 0; i < n; i++)
	{
		cool = va_arg(chains, char *);

		if (separator == 0 || i == n - 1) /*n-1 pour avoir le dernier entier*/
		{
			if (cool == NULL)
				printf("(nil)");
			else
				printf("%s", cool);
		}
		else
		{
			if (cool == NULL)
				printf("(nil)");
			else
				printf("%s%s", cool, separator);
		}
	}
	putchar('\n');
	va_end(chains);
}

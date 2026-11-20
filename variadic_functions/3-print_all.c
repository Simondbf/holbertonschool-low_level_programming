#include <stdio.h>
#include <stdarg.h>
#include "variadic_functions.h"

/**
 * print_all - affiche les valeurs de différents types
 * @format: constante <= 0
 * Return: pas car void
 */
void print_all(const char * const format, ...)
{
	va_list chain;
	int index = 0;
	char *fin = "", *string;
	const char *list = format;

	va_start(chain, format);
	while (list != NULL && list[index] != '\0')
	{
		switch (list[index])
		{
			case 'c':
				/*on ajoute la donnée de la séparation (fin) avant le texte imprimer*/
				printf("%s%c", fin, va_arg(chain, int));
				fin = ", ";
				break;
			case 'i':
				printf("%s%d", fin, va_arg(chain, int));
				fin = ", ";
				break;
			case 'f':
				printf("%s%f", fin, va_arg(chain, double));
				fin = ", ";
				break;
			case 's':
				string = va_arg(chain, char *);
				if (string == NULL)
				{
					printf("(nil)");
					break;
				}
				printf("%s%s", fin, string);
				fin = ", ";
				break;
		}
		index++;
	}
	putchar('\n');
	va_end(chain);
}

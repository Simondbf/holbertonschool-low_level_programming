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
	char *fin = ", ", *string;

	while (format == NULL)
	{
		putchar('\n');
		return;
	}
	va_start(chain, format);
	while (format[index] != '\0')
	{
		switch (format[index])
		{
			case 'c':
				printf("%c", va_arg(chain, int));
				break;
			case 'i':
				printf("%d", va_arg(chain, int));
				break;
			case 'f':
				printf("%f", va_arg(chain, double));
				break;
			case 's':
				string = va_arg(chain, char *);
				if (string == NULL)
				{
					printf("(nil)");
					break;
				}
				printf("%s", string);
				break;
			default:
				break;
		}
		if (format[index + 1] != '\0')
			printf("%s", fin);
		index++;
	}
	putchar('\n');
	va_end(chain);
}

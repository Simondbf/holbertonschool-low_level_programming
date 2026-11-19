#include <stdio.h>
#include <stdarg.h>
#include "variadic_functions.h"

/**
 * print_all - affiche les valeurs de différents types
 * @flormat: constante <= 0
 * Return: pas car void
 */
void print_all(const char * const format, ...)
{
	va_list chain;
	int index = 0;

	if (format == NULL)
	{
		putchar('\n');
		return;
	}

	va_start(chain, format);
	while (format[index] != '\0')
	{
		switch (format[index])
		{
			case 1:
				printf("%c\n", va_arg(chain, int));
				break;

			case 2:
				printf("%d", va_arg(chain, int));
				break;

			case 3:
				printf("%f", va_arg(chain, double));
				break;

			case 4:
				if (va_arg(chain, char *) == NULL)
					printf("(nil)");

				printf("%s", va_arg(chain, char *));
				break;

			default:
				break;
		}
		index++;
	}
	putchar('\n');
	va_end(chain);
}
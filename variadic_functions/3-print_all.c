#include <stdio.h>
#include <stdarg.h>
#include "variadic_functions.h"

/**
 * print_all - affiche 
 * @separator: chaine de caractère de séparation entre les nombres
 * @n: constante <= 0
 * Return: 0 si = null
 */
void print_all(const char * const format, ...)
{
	va_list chain;

	if (format == NULL)
		{
			putchar('\n');
			return (0);
		}

	va_start (chain, format);
	while ()
	{
		switch ()
		{
			case 1:

				break;
			case 2:

				break;
			case 3:
			
				break;
			case 4:
			
				break;
			case 5:
			
				break;
		}
	}
	putchar('\n');
	va_end(chain);
}
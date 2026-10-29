#include "main.h"
/**
 * _islower - lettr eminuscule ?
 * Return: 0
 */
int _islower(int c)
{
if (c >= 'a' && c <= 'z')
{
_putchar('1');
}
else
{
_putchar('0');
}
_putchar('\n');
}

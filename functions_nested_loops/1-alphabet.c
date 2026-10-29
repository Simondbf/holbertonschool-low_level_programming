#include "main.h"
/**
 * main - affiche l'alphabet en minuscules
 * Return: 0
 */
void print_alphabet(void)
{
int i;
for (i = 97; i <= 'z'; i++)
{
_putchar(i);
}
_putchar('\n');
}

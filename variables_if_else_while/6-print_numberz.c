#include <stdio.h>
/**
 * main - affiche tous les nombres à un seul chiffre en base 10
 * Return: 0
 */
int main(void)
{
short i;

for (i = 48; i < 58; i++)
{
putchar(i);
}
putchar('\n');
return (0);
}

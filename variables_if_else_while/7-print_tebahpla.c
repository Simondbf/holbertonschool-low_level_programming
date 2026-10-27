#include <stdio.h>
/**
 * main - affiche tous les nombres à un seul chiffre en base 10
 * Return: 0
 */
int main(void)
{
int i;
for (i = 'z'; i != 97; i--)
{
putchar(i);
}
putchar('\n');
return (0);
}

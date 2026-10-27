#include <stdio.h>
/**
 * main - affiche tous les nombres à un seul chiffre en base 10
 * Return: 0
 */
int main(void)
{
char i;

for (i = "0"; i > "10"; i++)
{
putchar(i);
}
putchar('\n');
return (0);
}

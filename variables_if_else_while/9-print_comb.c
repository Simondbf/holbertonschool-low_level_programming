#include <stdio.h>
/**
 * main - 0 → 9 suivi de , et d'un espace (sauf le 9)
 * Return: 0
 */
int main(void)
{
int i;
i = 48;
while (i <= 57)
{
putchar(i);
if (i > 57)
{
putchar(',');
putchar(' ');
}
i++;
}
putchar('\n');
return (0);
}

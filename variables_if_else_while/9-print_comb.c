#include <stdio.h>
/**
 * main - hexa en minuscule
 * Return: 0
 */
int main(void)
{
int i;
i = 49;
putchar('0');
while (i <= 57)
{
putchar(', ');
putchar(i);
i++;
}
putchar('\n');
return (0);
}

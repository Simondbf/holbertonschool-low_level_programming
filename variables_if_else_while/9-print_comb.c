#include <stdio.h>
/**
 * main - hexa en minuscule
 * Return: 0
 */
int main(void)
{
int i;
i = 48;
if (i <= 57)
{
putchar(i);
putchar(',');
putchar(' ');
i++;
}
else
putchar('\n');
return (0);
}

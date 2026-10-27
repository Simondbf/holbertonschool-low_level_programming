#include <stdio.h>
/**
 * main - hexa en minuscule
 * Return: 0
 */
int main(void)
{
int i;
i = 48;
while (i <= 57)
{
putchar(i);
putchar(',');
putchar(' ');
i++;
if (i > 57)
{
continue;
}
}
putchar('\n');
return (0);
}

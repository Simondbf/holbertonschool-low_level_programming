#include <stdio.h>
/**
 * main - entrée
 * Return: 0
 */
int main(void)
{
int n;
n = 97;
while (n <= 122)
{
putchar(n);
n++;
}
n = 41;
while (n <= 90)
{
putchar(n);
n++;
}
putchar('\n');
return (0);
}

#include <stdio.h>
/**
 * main - entrée
 * Return: 0
 */
int main(void)
{
int n;
int a;
n = 97;
a = 0;
while (n <= 122)
{
if (n == 101 || n == 113)
{
putchar(a);
}
putchar(n);
n++;
}
putchar('\n');
return (0);
}

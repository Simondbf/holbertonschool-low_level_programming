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
if (n != 101 && n != 113)
{
putchar(n);
n++;
continue;
}
}
putchar('\n');
return (0);
}

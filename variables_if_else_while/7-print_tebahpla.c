#include <stdio.h>
/**
 * main - alphabet minuscule z → a
 * Return: 0
 */
int main(void)
{
int i;
for (i = 'z'; i >= 97; i--)
{
putchar(i);
}
putchar('\n');
return (0);
}

#include <stdio.h>
/**
 * main - affiche alphabet sans e et q
 * Return: 0
 */
int main(void)
{
	int n;

	for (n = 97; n <= 122; n++)
	{
		if (n != 101 && n != 113)
		putchar(n);
	}
	putchar('\n');
return (0);
}

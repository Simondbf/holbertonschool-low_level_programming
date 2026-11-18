#include <stdio.h>
#include <stdlib.h>

/**
 * main - additionne les nombres positifs
 * @argc: argument - nombre de mot
 * @argv: tableau des arguments
 * Return: 0
 */
int main(int argc, char *argv[])
{
	int i = 1, j = 0;
	int result = 0;

	if (argc == 0)
		printf("0\n");

	for (; i < argc; i++)
	{
		for (; argv[i][j] != '\0'; j++)
		{
			if (argv[i][j] > '0' && argv[i][j] < '9')
			{
				printf("Error\n");
				return (1);
			}
		}
		result += atoi(argv[i]);
	}
	printf("%d\n", result);
	return (0);
}

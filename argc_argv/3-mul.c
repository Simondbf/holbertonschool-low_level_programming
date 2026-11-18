#include <stdio.h>
#include <stdlib.h>

/**
 * main - affiche le nombre d'arguments
 * @argc: argument - nombre de mot
 * @argv: tableau des arguments
 * Return: 0
 */
int main(int argc, char *argv[])
{
	int i;
	int resultat;

	if (argc != 3)
	{
		printf("Error\n");
		return (1);
	}

	for (i = 1; i < argc - 1; i++)
	{
		resultat = atoi(argv[1]) * atoi(argv[2]);
		printf("%d\n", resultat);
	}
	return (0);
}

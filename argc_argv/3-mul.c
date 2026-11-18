#include <stdio.h>
#include <stdlib.h>

/**
 * main - affiche le nombre d'arguments
 * programme qui écrit son propre nom
 * @argc: argument - nombre de mot
 * @argv: tableau des arguments
 * Return: 0
 */
int main(int argc, char *argv[])
{
	int i;
	int resultat;

	for (i = 1; i < argc - 1; i++)
	{
		if (i > 2)
		{
			printf("Error\n");
			return (1);
		}
		resultat = atoi(argv[1]) * atoi(argv[2]);
		printf("%d\n", resultat);
	}
	return (0);
}

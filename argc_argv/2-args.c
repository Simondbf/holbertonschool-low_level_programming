#include <stdio.h>

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

	for (i = 0; i < argc; i++)
		printf("%s\n", argv[i]);

	return (0);
}
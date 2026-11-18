#include <stdio.h>

/**
 * main - affiche le nombre d'arguments
 * @argc: argument - nombre de mot
 * @argv: tableau des arguments
 * Return: 0
 */
int main(int argc, char *argv[] __attribute__ ((unused)))
{
	int nbre_argument = argc - 1;

	printf("%d\n", nbre_argument);
	return (0);
}

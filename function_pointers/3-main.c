#include <stdio.h>
#include <stdlib.h>
#include "3-calc.h"

/**
 * main - calculatrice
 * @argc: nombres d'argument
 * @argv: tableau de texte
 * Return: 0
 */

int main(int argc, char *argv[])
{
	int a, b;
	int (*operation)(int, int); /*créer un pointeur vers une fonction int)*/

	/*s'il n'y a pas exactement 2 nombres + 1 opérateur = 3 arguments*/
	if (argc != 4)
	{
		printf("Error\n");
		printf("%d\n", argc);
		exit(98);
	}

	operation = get_op_func(argv[2]); /*récupère un pointeur opérateur*/

	a = atoi(argv[1]);
	b = atoi(argv[3]);

	/*s'in n'y a pas l'un des opérateur correctement écrit*/
	if (operation == NULL)
	{
		printf("Error\n");
		exit(99);
	}

	/*s'in n'y a pas l'un des opérateur correctement écrit*/
	else if (b == 0 && (operation == op_div || operation == op_mod))
	{
		printf("Error\n");
		exit(100);
	}

	else
		printf("%d\n", operation(a, b)); /*operation car récupère pointeur de get*/

	return (0);
}

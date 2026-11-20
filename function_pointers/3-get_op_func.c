#include <stdio.h>
#include <string.h> /*permet d'avoir strcmp (comparaison)*/
#include "3-calc.h"

/**
 * get_op_func - recherche la bonne opération à faire via une comparaison
 * @s: l'opérateur donner par l'user
 * Return: la bonne fonction à appeler
 */

int (*get_op_func(char *s))(int, int)
{
	op_t ops[] = {
		{"+", op_add},
		{"-", op_sub},
		{"*", op_mul},
		{"/", op_div},
		{"%", op_mod},
		{NULL, NULL}
	};
	int i = 0;

	while (i < 5)
	{
		if (strcmp(s, ops[i].op) == 0) /*fonction de comparaison* - stdio*/
			return (ops[i].f);

		i++;
	}
	return (NULL);
}

#include <stdio.h>
#include <stddef.h>
#include "lists.h"

/**
 * print_list - affiche tous les élement d'une liste chainée
 * @list_t: type de la liste
 * @h: pointeur vers le début de la liste
 * Return: nombre de noeuds dans la listes
 */

size_t print_list(const list_t *h)
{
	size_t i;

	if (h == NULL)
		printf("[0] (nil)\n");

	for (i = 0; h != NULL ; i++)
		{
			h = h->next;
			printf("%lu\n", i);
		}

	return (i);
}

#include <stdio.h>
#include <stddef.h>
#include "lists.h"

/**
 * list_len - affiche la longueur d'une liste chainée
 * @h: pointeur vers le début de la liste
 * Return: la longueur de la liste
 */

size_t list_len(const list_t *h)
{
	size_t i;

	for (i = 0; h != NULL; i++)
		h = h->next;

	return (i);
}
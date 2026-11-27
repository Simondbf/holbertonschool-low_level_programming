#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "lists.h"

/**
* free_list - libérer l'ensemble de la mémoire prise
* @head: pointeur vers le 1er noeud
* Return: la longueur de la liste
*/

void free_list(list_t *head)
{
	list_t *node;

	free(node);
	node = NULL;
}
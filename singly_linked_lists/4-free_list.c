#include <stdlib.h>
#include "lists.h"

/**
 * free_list - libère une liste chaînée
 * @head: pointeur vers le 1er noeud
 */
void free_list(list_t *head)
{
	list_t *temp;

	while (head != NULL)
	{
		temp = head->next;
		free(head->str);
		free(head);
		head = temp;
	}
}

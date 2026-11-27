#include <stdio.h>
#include <stddef.h>
#include <string.h> /*pour utiliser strdup*/
#include <stdlib.h>
#include "lists.h"

/**
* add_node_end - ajoute un noeud à la fin de la liste
* @head: double pointeur vers le dernier noeud
* @str: chaine de caractère
* Return: la longueur de la liste
*/

list_t *add_node_end(list_t **head, const char *str)
{
	list_t *node;
	list_t *temp;
	int len_temp;

	if (head == NULL || str == NULL)
		return (NULL);
	/* Réservation du noeud dans le tas pour qu'il survive à la fonction*/
	node  = malloc(sizeof(list_t));
	if (node == NULL)
	{
		free(node);
		return (NULL);
	}
	/*copie du nom + libération si échec*/
	node->str = strdup(str);
	if (node->str == NULL)
	{
		free(node->str);
		free(node);
		return (NULL);
	}
	/*connaitre la longueur de la chaine*/
	for (len_temp = 0; str[len_temp] != '\0'; len_temp++)
	{	}
	/*initialisation du nouveau noeud se plaçant à la fin*/
	node->len = len_temp;
	node->next = NULL;
	/*si c'est la liste est vide, il prends la tête la liste*/
	if (*head == NULL)
	{
		*head = node;
		return (node);
	}
	/*sinon il se met derrière*/
	/*variable temporaire obligatoire pour ne pas perdre l'adresse du 1er noeud*/
	temp = *head;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = node;
	return (node);
}

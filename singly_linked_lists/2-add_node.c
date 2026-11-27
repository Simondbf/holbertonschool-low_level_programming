#include <stdio.h>
#include <stddef.h>
#include <string.h> /*pour utiliser strdup*/
#include <stdlib.h>
#include "lists.h"

/**
* add_node - ajout d'un nouveau noeud au début de la liste
* @head: double pointeur vers le 1er noeud
* @str: chaine de caractère
* Return: la longueur de la liste
*/

list_t *add_node(list_t **head, const char *str)
{
	list_t *node;
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

	/*enregistrement du compteur dans la longueur du noeud*/
	node->len = len_temp;
	
	/* lien entre le 1er noeud et le 2e*/
	node->next = *head;

	/*modifier l'adresse de head pour qu'il passe sur le 1er noeuf*/
	*head = node;

	return (node);
}

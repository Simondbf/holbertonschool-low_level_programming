#ifndef LISTS_H
#define LISTS_H

/**
 * struct list_s - liste simplement chaînée
 * @str: chaîne de caractères (allouée avec malloc)
 * @len: longueur de la chaîne de caractères
 * @next: pointe vers le nœud suivant
 *
 * Description: structure d'un nœud de liste simplement chaînée
 */
typedef struct list_s
{
    char *str;
    unsigned int len;
    struct list_s *next;
} list_t;

size_t print_list(const list_t *h);

#endif

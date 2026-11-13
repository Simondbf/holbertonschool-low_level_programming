#ifndef DOG_H
#define DOG_H
/**
 * struct dog - informations sur un chien
 * @name: nom du chien
 * @age: âge du chien
 * @owner: nom du propriétaire
 * Description: structure qui regroupe les informations d'un chien
 */
typedef struct dog
{
	char *name;
	float age;
	char *owner;
} my_dog;

void init_dog(struct dog *d, char *name, float age, char *owner);

#endif

#ifndef DOG_H
#define DOG_H
/**
 * struct dog - informations sur un chien
 * @name: nom du chien
 * @age: âge du chien
 * @owner: nom du propriétaire
 * Description: structure qui regroupe les informations d'un chien
 */
struct dog
{
	char *name;
	float age;
	char *owner;
};
typedef struct dog dog_t;

/*
* ou sinon:
* typedef struct dog {
*	char *name;
*	float age;
*	char *owner;
* }dog_t;
*/

void init_dog(struct dog *d, char *name, float age, char *owner);
void print_dog(struct dog *d);

#endif

#include "main.h"
/**
 * *string_toupper - change les minuscules en majuscules
 * Return: 0
 * @str: pointeur
 */
char *string_toupper(char *str)
{
	int i = 0;

	for (; str[i] != '\0'; i++)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] -= 32;
		}
	}
	return (str);
}

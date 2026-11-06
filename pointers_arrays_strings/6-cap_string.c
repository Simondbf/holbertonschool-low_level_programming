#include "main.h"
/**
 * *string_toupper - majuscule sur la 1ere lettre de chaque mot
 * Return: 0
 * @str: pointeur
 */
char *cap_string(char *str)
{
	int i = 0;

	for (; str[i] != '\0';i++)
	{
		if (str[i] == ' ' || str[i] == '\t')
		{
			if (str[i + 1] >= 'a' && str[i + 1] <= 'z')
				str[i + 1] += 32;
		}
		if (str[i] == '\n' || str[i] == ',')
		{
			if (str[i + 1] >= 'a' && str[i + 1] <= 'z')
				str[i + 1] += 32;
		}
		if (str[i] == ';' || str[i] == '.')
		{
			if (str[i + 1] >= 'a' && str[i + 1] <= 'z')
				str[i + 1] += 32;
		}
		if (str[i] == '!' || str[i] == '?' || str[i] == '"')
		{
			if (str[i + 1] >= 'a' && str[i + 1] <= 'z')
				str[i + 1] += 32;
		}
		if (str[i] == '(' || str[i] == ')')
		{
			if (str[i + 1] >= 'a' && str[i + 1] <= 'z')
				str[i + 1] += 32;
		}
		if (str[i] == '{' || str[i] == '}')
		{
			if (str[i + 1] >= 'a' && str[i + 1] <= 'z')
				str[i + 1] += 32;
		}
	}
	return (str);
}

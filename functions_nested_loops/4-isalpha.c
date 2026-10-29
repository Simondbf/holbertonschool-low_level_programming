#include "main.h"
/**
 * _isalpha - lettre de l'alphabet ?
 * @c: caractère à vérifier
 * Return: 0
 */
int _isalpha(int c)
{
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
	{
		return (1);
	}
	else
	{
		return (0);
	}
	_putchar('\n');
}

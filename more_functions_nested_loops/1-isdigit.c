#include "main.h"
/**
 * _isdigit - chiffre (0 à 9) ?
 * Return: 0
 * @c: lettre
*/
int _isdigit(int c)
{
	if (c >= 48 && c <= 57)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

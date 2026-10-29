#include "main.h"
/**
 *  _abs - valeur asbolue d'un nombre
 * @n: nombre à vérifier
 * Return: 0
 */
int _abs(int n)
{
	if (n > 0)
	{
		return (n);
	}
	else
	{
		return (-n);
	}
	_putchar('\n');
	return (0);
}

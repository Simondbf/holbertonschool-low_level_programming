#include "main.h"
/**
 * _isupper - majuscule ?
 * Return: 0
 * @c: lettre
*/
int _isupper(int c)
{
	if (c >= 65 && c < 'Z')
	{
		return (1);
	}
	else
	{
		return (0);
	}
}
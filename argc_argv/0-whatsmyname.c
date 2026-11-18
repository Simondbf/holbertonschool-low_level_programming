#include <stdio.h>

/**
 * main - découverte de argc et argv
 * programme qui écrit son propre nom
 * @argc: argument - nombre de mot
 * @argv: tableau des arguments
 * return: 0
 */
int main(int argc __attribute__ ((unused)), char * argv[])
{
	printf("%s\n", argv[0]);

	return (0);
}

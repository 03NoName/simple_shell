#include "sHell.h"

int scriptmode(char *av[]);
/**
 * main - Runs shell.
 * @ac: Number of args.
 * @av: Cmd line for arg matrix.
 * @environ: Env matrix.
 *
 * Return: Return val of last cmd.
 */

int main(int ac, char *av[], char **environ);
int main(int ac, char *av[], char **environ)
{
	int ret = 0;

	char *pidptr;

	initsvars(ac - 1, av);
	pidptr = _getpid();
	setsvar("$", pidptr);
	free(pidptr);
	_getline(NULL, -2);

	setallenv(environ, NULL);
	if (ac > 1)
		ret = scriptmode(av);
	else
		ret = shintmode();
	exitcleanup(NULL);
	exit_hist();
	return (ret);
}

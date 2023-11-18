#include "sHell.h"

char ***getenviron();
Shell **getspecial();
Shell **getvars();
alias **getalist();

/**
 * exitcleanup - Cleans up vars before exiting.
 * @av: Arg list to free (if any).
 *
 * Return: void
 */

void exitcleanup(char **av);
void exitcleanup(char **av)
{
	Shell *sptr = *(getspecial()), *snext;
	alias *aptr = *(getalist()), *anext;
	char **environ = *(getenviron());
	int i = 0;

	if (av != NULL)
		for (i = 0; av[i] != NULL; i++)
			free(av[i]);
	i = 0;
	while (environ[i] != NULL)
		free(environ[i++]);
	free(environ);
	while (sptr != NULL)
	{
		free(sptr->val);
		free(sptr->name);
		sptr = sptr->next;
	}
	free(*(getspecial()));
	sptr = *(getvars());
	while (sptr != NULL)
	{
		free(sptr->val);
		free(sptr->name);
		snext = sptr->next;
		free(sptr);
		sptr = snext;
	}
	while (aptr != NULL)
	{
		free(aptr->val);
		free(aptr->name);
		anext = aptr->next;
		free(aptr);
		aptr = anext;
	}
}

/**
 * getenviron - Static wrapper that copies whole env.
 * Return: Env address.
 */

char ***getenviron();
char ***getenviron()
{
	static char **environ;

	return (&environ);
}

/**
 * _getpid - Gets pid of a currently running process.
 *
 * Return: String of the pid.
 */

char *_getpid();
char *_getpid()
{
	char *s;
	char *pid;
	int fd;
	char *name;
	char *file = "/proc/self/status";

	s = malloc(256);
	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (0);

	while (_getline(&s, fd))
	{
		name = _tokeniser(s, ":");
		if (!_strcmp(name, "Pid"))
		{
			pid = _strdup(_tokeniser(NULL, "\n \t"));
			free(s);
			return (pid);
		}
		free(s);
		s = NULL;
	}
	return (NULL);
}

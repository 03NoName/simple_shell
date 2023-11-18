#include "sHell.h"

/**
 * inputvalidator - Validates input buffer.
 *
 * Complete is bit flag type to complete.
 * 0 none, 1 single quote, 2 double quote, 4 logic,
 * fd is fd we used to get input the first time
 *
 *
 * @buf: Buffer
 * @fd: File description.
 *
 * Return: Return value of command, 2 for syntax errors,
 * or negative numbers for syscall errors.
 */

int inputvalidator(char **buf, int fd);
int inputvalidator(char **buf, int fd)
{
	char *newbuf, *bufgl, *bufptr = *buf;
	ssize_t lenr;
	size_t lenbuf;
	int start = 1;
	int complete = 0;

	linecount(1);
	if (*bufptr == 0)
		return (0);
	while (*bufptr)
	{
		while ((*bufptr == ' ' || *bufptr == '\n') && !(complete & 3))
			bufptr++;
		if (*bufptr == 0)
			break;
		if (start)
		{
			if (*bufptr == ';' && *(bufptr + 1) == ';')
			{
				free(*buf);
				_errorhandle(": Syntax error \";;\" unexpected\n");
				setsvar("0", "2");
				return (2);
			}
			if (*bufptr == ';')
			{
				free(*buf);
				_errorhandle(": Syntax error \";\" unexpected\n");
				setsvar("0", "2");
				return (2);
			}
			if (*bufptr == '&' && *(bufptr + 1) == '&')
			{
				free(*buf);
				_errorhandle(": Syntax error \"&&\" unexpected\n");
				setsvar("0", "2");
				return (2);
			}
			if (*bufptr == '&')
			{
				free(*buf);
				_errorhandle(": Syntax error \"&\" unexpected\n");
				setsvar("0", "2");
				return (2);
			}
			if (*bufptr == '|' && *(bufptr + 1) == '|')
			{
				free(*buf);
				_errorhandle(": syntax error \"||\" unexpected\n");
				setsvar("0", "2");
				return (2);
			}
			if (*bufptr == '|')
			{
				free(*buf);
				_errorhandle(": syntax error \"|\" unexpected\n");
				setsvar("0", "2");
				return (2);
			}
			start = 0;
		}
		if (bufptr[0] == '\n' && bufptr[1] == 0)
			break;
		if (*bufptr == '#' &&
				!(complete & 3) &&
				(bufptr == *buf || *(bufptr - 1) == ' '))
		{
			*bufptr = 0;
			break;
		}
		complete &= ~4;
		if (*bufptr == '"' && !(complete & 3))
		{
			complete |= 2;
			bufptr++;
			continue;
		}
		if (*bufptr == '"' && complete & 2)
			complete &= ~2;
		if (*bufptr == '\'' && !(complete & 3))
		{
			complete |= 1;
			bufptr++;
			continue;
		}
		if (*bufptr == '\'' && complete & 1)
			complete &= ~1;
		if (bufptr[0] == '&' && !(complete & 3))
		{
			if (bufptr[1] == '&')
			{
				complete |= 4;
				start = 1;
				bufptr++;
			}
			else if (bufptr[1] == '|')
			{
				free(*buf);
				_errorhandle(": syntax error \"|\" unexpected\n");
				setsvar("0", "2");
				return (2);
			}
			else if (bufptr[1] == ';')
			{
				free(*buf);
				_errorhandle(": syntax error \";\" unexpected\n");
				setsvar("0", "2");
				return (2);
			}
		}
		if ((bufptr[0] == '|') && !(complete & 3))
		{
			if (bufptr[1] == '|')
			{
				complete |= 4;
				start = 1;
				bufptr++;
			}
			else if (bufptr[1] == ';')
			{
				free(*buf);
				_errorhandle(": syntax error \";\" unexpected\n");
				setsvar("0", "2");
				return (2);
			}
			else if (bufptr[1] == '&')
			{
				free(*buf);
				_errorhandle(": syntax error \"&\" unexpected\n");
				setsvar("0", "2");
				return (2);
			}
		}
		if (*bufptr == ';')
		{
			if (*(bufptr + 1) == ';')
			{
				free(*buf);
				_errorhandle(": Syntax error \";;\" unexpected\n");
				setsvar("0", "2");
				return (2);
			}
			start = 1;
		}
		bufptr++;
	}
	if (complete & 7)
	{
		bufgl = NULL;
		if (isatty(fd))
			fprintstrs(1, ">", NULL);
		lenr = _getline(&bufgl, fd);
		if (lenr == 0 && !isatty(fd))
		{
			free(buf);
			free(bufgl);
			_errorhandle(": Syntax error: unterminated quoted string\n");
			return (-1);
		}
		lenbuf = _strlen(*buf);
		newbuf = malloc(lenbuf + lenr + 1);
		forcopy(newbuf, *buf);
		forcopy(newbuf + lenbuf, bufgl);
		free(*buf);
		free(bufgl);
		return (inputvalidator(&newbuf, fd));
	}
	sethist(*buf);
	return (parseargs(buf));
}

/**
 * shintmode - Shell interactive mode.
 *
 * Return: 0.
 */

int shintmode(void);
int shintmode(void)
{
	char *bufgl = NULL, *pwd;
	ssize_t lenr = 0, eofflag = 0, ret = 0;
	int istty = isatty(0) && isatty(1);

	while (!eofflag)
	{
		if (istty)
		{
			pwd = _getenv("PWD");
			if (pwd != NULL)
			{
				fprintstrs(1, "n_tuser1", pwd, "$ ", NULL);
				free(pwd);
			}
			else
			{
				fprintstrs(1, "n_tuser1🦦", NULL);
			}
		}
		lenr = _getline(&bufgl, STDIN_FILENO);
		if (lenr == 0 || lenr == -1)
		{
			free(bufgl);
			break;
		}
		if (bufgl[lenr - 1] != '\n')
			eofflag = 1;
		ret = inputvalidator(&bufgl, STDIN_FILENO);
		bufgl = NULL;
		if (eofflag)
			break;
	}
	return (ret);
}
/**
 * scriptmode - Shell script mode.
 * @av: Arg.
 *
 * Return: 0 upon success or -1 for error.
 */

int scriptmode(char *av[]);
int scriptmode(char *av[])
{
	char *bufgl = NULL;
	int infile;
	ssize_t lenr = 0, eofflag = 0, ret = 0;

	infile = open(av[1], O_RDONLY);
	if (infile == -1)
	{
		fprintstrs(STDERR_FILENO, av[0], ": 0: Can't open ",
				av[1], "\n", NULL);
		return (127);
	}
	while (!eofflag)
	{
		lenr = _getline(&bufgl, infile);
		if (lenr == 0 || lenr == -1)
		{
			free(bufgl);
			break;
		}
		if (bufgl[lenr - 1] != '\n')
			eofflag = 1;
		ret = inputvalidator(&bufgl, STDIN_FILENO);
		bufgl = NULL;
		if (eofflag)
			break;
	}
	close(infile);
	return (ret);
}
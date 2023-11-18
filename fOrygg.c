#include "sHell.h"

/**
 * _atoi - Convert strings to number.
 * @s: Pointer int.
 * _atoi: Converts string to number.
 *
 * Return: The result (converted number).
 */

int _atoi(char *s);
int _atoi(char *s)
{
	int i;
	int check_num;
	unsigned int sum;
	unsigned int x;
	int neg;

	neg = 0;
	check_num = 0;
	sum = 0;
	i = 0;

	while (s[i] != '\0')
	{
		if ((s[i] > '9' || s[i] < '0') && check_num > 0)
			break;
		if (s[i] == '-')
			neg++;
		if (s[i] >= '0' && s[i] <= '9')
			check_num++;
		i++;
	}
	i = i - 1;
	x = 1;
	while (check_num > 0)
	{
		sum = sum + ((s[i] - '0') * x);
		x = x * 10;
		i--;
		check_num--;
	}
	if (neg % 2 != 0)
		sum = sum * -1;
	return (sum);
}

/**
 * getallenv - Gets all env.
 *
 * Return: Env.
 */

char **getallenv();
char **getallenv()
{
	char **environ = *(getenviron());
	char **envcopy;
	size_t len = 0;

	envcopy = environ;
	while (envcopy[len] != NULL)
		len++;

	envcopy = malloc(sizeof(char **) * (len + 1));
	if (envcopy == NULL)
		return (NULL);
	while (len > 0)
	{
		envcopy[len] = environ[len];
		len--;
	}
	envcopy[len] = environ[len];
	return (envcopy);
}

/**
 * setallenv - Set whole environment for new value.
 * @envin: Env.
 * @newval: New value to be added.
 *
 * Return: 0 if success, -1 if failure.
 */

/* Add should be null for first init to not free passed array */
int setallenv(char **envin, char *newval);
int setallenv(char **envin, char *newval)
{
	char ***environ = getenviron();
	size_t len = 0;

	while (envin[len] != NULL)
		len++;
	if (newval != NULL)
		len++;
	*environ = malloc(sizeof(char **) * (len + 1));
	if (*environ == NULL)
		return (-1);
	for (len = 0; envin[len] != NULL; len++)
		if (newval == NULL)
		{
			(*environ)[len] = _strdup(envin[len]);
		}
		else
			(*environ)[len] = envin[len];
	if (newval != NULL)
	{
		(*environ)[len] = newval;
		len++;
	}
	(*environ)[len] = NULL;
	if (newval != NULL)
		free(envin);
	return (0);
}

/**
 * _getenv - Get local env.
 * @name: Env variable.
 *
 * Return: String of local env.
 */

char *_getenv(char *name);
char *_getenv(char *name)
{
	char **environ = *(getenviron());
	int i, j;
	char *s;

	i = 0;
	while (environ[i] != NULL)
	{
		s = environ[i];
		j = 0;

		while (s[j] == name[j])
		{
			j++;
			if (name[j] == 0 && s[j] == '=')
				return (_strdup(s + j + 1));
		}
		i++;
	}
	return (name);
}

/**
 * _setenv - Set env for new value.
 * @name: Name of var.
 * @val: Value of var.
 *
 * Return: 0 or setallenv if success, -1 if fail.
 */

int _setenv(char *name, char *val);
int _setenv(char *name, char *val)
{
	char ***environroot = getenviron();
	char **environ = *environroot;
	int i, j, namel, vall;
	char *s, *ptr;

	if (name == NULL || val == NULL)
		return (0);
	namel = _strlen(name);
	vall = _strlen(val);
	ptr = malloc(sizeof(char) * (namel + vall + 2));
	if (ptr == NULL)
		return (-1);
	s = ptr;
	forcopy(s, name);
	s += namel;
	forcopy(s++, "=");
	forcopy(s, val);
	s += vall;
	*s = 0;
	i = 0;
	while (environ[i] != NULL)
	{
		s = environ[i];
		j = 0;
		while (s[j] == name[j])
		{
			j++;
			if (name[j] == 0 && s[j] == '=')
			{
				free(environ[i]);
				environ[i] = ptr;
				return (0);
			}
		}
		i++;
	}
	return (setallenv(*environroot, ptr));
}

/**
 * _unsetenv - Unset env.
 * @name: Name of variable to unset.
 * Return: 0 if sucess.
 *
 * Testing functionality copy environ, if hits skip over, realloc.
 */

int _unsetenv(char *name);
int _unsetenv(char *name)
{
	char **environ = *getenviron();
	int i, j;
	int check = 0;
	char *s;
	char **env;

	if (name == NULL)
		return (0);

	i = 0;
	while (environ[i] != NULL)
	{
		s = environ[i];
		j = 0;
		while (s[j] == name[j])
		{
			j++;
			if (s[j] == '=' && name[j] == '\0')
			{
				check = 1;
				break;
			}
		}
		if (check == 1)
			break;
		i++;
	}
	free(environ[i]);
	while (environ[i] != NULL)
	{
		environ[i] = environ[i + 1];
		i++;
	}
	environ[i] = NULL;
	env = environ;
	setallenv(env, NULL);
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
	return (0);
}

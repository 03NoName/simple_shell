#include "sHell.h"

/**
 * initsvars - Initialize vars.
 * @ac: Arg integer.
 * @av: String arg.
 *
 * Return: Int.
 */

int initsvars(int ac, char **av);
int initsvars(int ac, char **av)
{
	Shell **specialroot = getspecial();
	Shell *special;
	Shell *ptr;
	int i = 0;
	char nums[2] = {0, 0};

	*specialroot = malloc(sizeof(Shell) * 15);
	if (*specialroot == NULL)
		return (-1);
	special = *specialroot;
	special->val = _strdup("0");
	special->name = _strdup("?");
	ptr = special + 1;
	special->next = ptr;
	while (av[i] != NULL)
	{
		nums[0] = i + '0';
		ptr->val = _strdup(av[i]);
		ptr->name = _strdup(nums);
		ptr->next = ptr + 1;
		ptr = ptr->next;
		i++;
	}
	while (i < 10)
	{
		nums[0] = i + '0';
		ptr->val = _strdup("0");
		ptr->name = _strdup(nums);
		ptr->next = ptr + 1;
		ptr = ptr->next;
		i++;
	}
	ptr->name = _strdup("$");
	ptr->val = _strdup("0");
	ptr->next = ptr + 1;
	ptr = ptr->next;
	ptr->name = _strdup("#");
	ptr->val = itos(ac);
	ptr->next = NULL;
	return (0);
}


/**
 * getsvar - Gets shell var.
 * @name: Name of shell var.
 * Return: Original arg if not found.
 */

char *getsvar(char *name);
char *getsvar(char *name)
{
	Shell *special = *(getspecial()), *vars = *(getvars());
	Shell *ptr = special;

	while (ptr != NULL && _strcmp(ptr->name, name))
	{
		ptr = ptr->next;
	}
	if (ptr != NULL)
	{
		return (_strdup(ptr->val));
	}
	ptr = vars;
	while (ptr != NULL && _strcmp(ptr->name, name))
	{
		ptr = ptr->next;
	}
	if (ptr == NULL)
	{
		return (name);
	}
	return (_strdup(ptr->val));
}

/**
 * setsvar - Sets shell var.
 * @name: Name of var.
 * @val: Value of var.
 *
 * Return: Int.
 */

int setsvar(char *name, char *val);
int setsvar(char *name, char *val)
{
	Shell **vars = getvars();
	Shell *special = *(getspecial());
	Shell *ptr = special, *new;

	while (_strcmp(ptr->name, name) && ptr->next != NULL)
	{
		ptr = ptr->next;
	}
	if (!_strcmp(ptr->name, name))
	{
		free(ptr->val);
		ptr->val = _strdup(val);
		return (0);
	}
	ptr = *vars;
	if (ptr == NULL)
	{
		new = malloc(sizeof(Shell));
		if (new == NULL)
			return (-1);
		new->name = _strdup(name);
		new->val = _strdup(val);
		new->next = NULL;
		*vars = new;
		return (0);
	}
	while (_strcmp(ptr->name, name) && ptr->next != NULL)
		ptr = ptr->next;
	if (ptr != NULL && !_strcmp(ptr->name, name))
	{
		free(ptr->val);
		ptr->val = _strdup(val);
	}
	else
	{
		new = malloc(sizeof(Shell));
		if (new == NULL)
			return (-1);
		new->name = _strdup(name);
		new->val = _strdup(val);
		new->next = NULL;
		ptr->next = new;
	}
	return (0);
}

/**
 * unsetsvar - Unset shell var.
 * @name: Name of var to unset.
 *
 * Return: Int.
 */

int unsetsvar(char *name);
int unsetsvar(char *name)
{
	Shell *vars = *getvars();
	Shell *ptr = vars, *next;

	if (vars == NULL)
		return (0);
	if (!_strcmp(ptr->name, name))
	{
		*vars = *vars->next;
		free(ptr->name);
		free(ptr->val);
		free(ptr);
		return (0);
	}
	while (ptr->next != NULL && _strcmp(ptr->next->name, name))
		ptr = ptr->next;
	if (!_strcmp(ptr->next->name, name))
	{
		next = ptr->next->next;
		free(ptr->next->name);
		free(ptr->next->val);
		free(ptr->next);
		ptr->next = next;
	}
	return (0);
}

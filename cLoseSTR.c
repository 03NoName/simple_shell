#include "sHell.h"

/**
 * _strcmp - Compares two or all strings.
 * @s1: Char pointer.
 * @s2: Char pointer.
 * _strcmp: Compares two strings at the same spot on each string.
 *
 * Return: Positive, negative, or 0 if theyre the same.
 */

int _strcmp(char *s1, char *s2);
int _strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
		{
			return (s1[i] - s2[i]);
		}
		i++;
	}
	return (s1[i] - s2[i]);
}

/**
 * _strlen - Counts length of strings.
 * @str: String.
 *
 * Return: Length of string.
 */

size_t _strlen(char *str);
size_t _strlen(char *str)
{
	size_t length = 0;

	while (*str++)
		length++;

	return (length);
}

/**
 * forcopy - String cpy.
 * @dest: Dest string.
 * @src: Src string.
 *
 * Return: String.
 */

char *forcopy(char *dest, char *src);
char *forcopy(char *dest, char *src)
{
	char *ptr = dest;

	while (*src)
		*ptr++ = *src++;
	*ptr = *src;

	return (dest);
}

/**
 * _strdup - String duplication.
 * @str: String.
 *
 * Return: String
 */

char *_strdup(char *str);
char *_strdup(char *str)
{
	char *new;

	if (str == NULL)
		return (NULL);
	new = malloc(sizeof(char) * (_strlen(str) + 1));
	if (new == NULL)
		return (NULL);
	forcopy(new, str);
	return (new);
}

/**
 * _strcat - String concatenation.
 * @dest: Destination.
 * @src: Source.
 *
 * Return: Concatenated string.
 */

char *_strcat(char *dest, char *src);
char *_strcat(char *dest, char *src)
{
	char *ptrs = dest;

	while (*ptrs)
		ptrs++;
	while (*src)
		*ptrs++ = *src++;
	*ptrs = 0;
	return (dest);
}

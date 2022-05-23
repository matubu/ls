#pragma once

#include <unistd.h>

size_t	len(const char *s)
{
	size_t	len = 0;

	while (*s)
	{
		++len;
		++s;
	}
	return (len);
}

int	ft_strcmp(const char *a, const char *b)
{
	while (*a && *a == *b)
	{
		++a;
		++b;
	}
	return (*(unsigned char *)a - *(unsigned char *)b);
}

int	putch(char c)
{
	return write(1, &c, 1);
}

int	put(const char *s)
{
	return write(1, s, len(s));
}

int	puts(const char *s)
{
	return put(s) + putch('\n');
}
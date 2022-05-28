#pragma once

#include "utils.h"
#include <stdlib.h>

void	*ft_malloc(size_t size)
{
	void	*new = malloc(size);

	if (new == NULL)
	{
		puts("ls: allocation error");
		exit(1);
	}
	return (new);
}

char	*ft_strdup(const char *s)
{
	char	*new = ft_malloc(len(s) + 1);
	char	*ptr = new;
	while ((*ptr++ = *s++));
	return (new);
}
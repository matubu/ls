#pragma once

#include "utils.h"
#include <stdlib.h>

void	allocationError(void) {
	puts("ls: allocation error");
	exit(1);
}

void	*ft_malloc(size_t size)
{
	void	*new = malloc(size);

	if (new == NULL) allocationError();
	return (new);
}

char	*ft_strdup(const char *s)
{
	char	*new = ft_malloc(len(s) + 1);
	char	*ptr = new;
	while ((*ptr++ = *s++));
	return (new);
}
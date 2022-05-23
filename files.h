#pragma once

#include "mem.h"
#include "utils.h"

#include <sys/stat.h>

typedef struct {
	struct stat	stat;
	char		*name;
	char		*path;
} File;

typedef struct {
	File	*files;
	int		count;
}	FileList;

char	*joinpath(char *a, char *b)
{
	int len_a = a ? len(a) : 0;
	if (len_a == 0)
		return (ft_strdup(b));

	char	*s = ft_malloc(sizeof(char) * (len_a + len(b) + 2));
	char	*ptr = s;

	while (*a)
		*ptr++ = *a++;

	*ptr++ = '/';

	while ((*ptr++ = *b++));
	return (s);
}
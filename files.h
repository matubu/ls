#pragma once

#include "opts.h"
#include "mem.h"
#include "utils.h"

#include <sys/errno.h>
#include <string.h>

typedef struct {
	struct stat		stat;
	char			*name;
	char			*path;
	struct timespec	time;
} File;

typedef struct {
	File	*files;
	int		count;
	int		cap;
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

void	pushFile(FileList *ls, char *path, char *name, t_opts *opts)
{
	struct stat buf;

	if (lstat(path, &buf) == -1)
	{
		put("ls: "); put(name); put(": "); put(strerror(errno)); putch('\n');
		free(path);
		return ;
	}

	int i = ls->count++;
	if (ls->count > ls->cap)
	{
		ls->cap = ls->cap ? ls->cap << 2 : 16;
		File	*tmp = ft_malloc(sizeof(File) * ls->cap);

		for (int j = i; j--;)
			tmp[j] = ls->files[j];

		free(ls->files);
		ls->files = tmp;
	}

	ls->files[i].stat = buf;
	ls->files[i].name = ft_strdup(name);
	ls->files[i].path = path;

	ls->files[i].time = opts->flags & use_access_time
		? ls->files[i].stat.st_atimespec
		: ls->files[i].stat.st_mtimespec;
}

void	freeFileList(FileList *file_list)
{
	while (file_list->count--)
	{
		free(file_list->files[file_list->count].name);
		free(file_list->files[file_list->count].path);
	}
	free(file_list->files);
	file_list->files = NULL;
}
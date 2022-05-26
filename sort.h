#pragma once

#include "utils.h"
#include "files.h"

typedef long long (*sorting_func_t)(const File *, const File *);

long long sort_ascending(const File *a, const File *b)
{
	return (ft_strcmp(a->name, b->name));
}

long long sort_modified(const File *a, const File *b)
{
	long long d = b->time.tv_sec - a->time.tv_sec;
	if (d) return (d);
	return (sort_ascending(a, b));
}

void sortFileList(FileList *file_list, sorting_func_t sort)
{
	int	notsorted = 1;
	File *files = file_list->files;

	for (int i = 0; i < file_list->count - 1; ++i)
	{
		notsorted = 0;
		int min = i;
		for (int j = i; ++j < file_list->count;)
			if (sort(files + min, files + j) > 0)
				min = j;

		File tmp = files[i];
		files[i] = files[min];
		files[min] = tmp;
	}
}
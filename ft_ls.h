#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/errno.h>

typedef enum {
	long_format         = 1<<1, // -l show more info
	recusive            = 1<<2, // -R recusive files
	show_all            = 1<<3, // -a show dot files
	colors              = 1<<4, // -G enable coloring
	show_group_name     = 1<<5, // -g hide owner name (enable -l)
	only_current_dir    = 1<<6, // -d only show the current directory

	no_sort             = 1<<7, // -f disable sorting (enable -a)
	descending_order    = 1<<8, // -r sort by reverse alphabetic order
	time_modified_order = 1<<9, // -t sort by time modified
	time_access_order   = 1<<10,// -u sort by last access
}	t_flags;

typedef struct {
	t_flags	flags;
	char	**files;
}	t_opts;

const t_flags	flags_map[256] = {
	['l'] = long_format,
	['R'] = recusive,
	['a'] = show_all,
	['G'] = colors,
	['g'] = long_format | show_group_name,
	['d'] = only_current_dir,

	['f'] = show_all | no_sort,
	['r'] = descending_order,
	['t'] = time_modified_order,
	['u'] = time_access_order,
};
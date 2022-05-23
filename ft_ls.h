#pragma once

#include "mem.h"
#include "files.h"
#include "sort.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/errno.h>
#include <time.h>

typedef enum {
	sorting_flag        = 3,
	ascending_order     = 0, // default
	time_modified_order = 1, // -t sort by time modified
	time_access_order   = 2, // -u sort by last access

	no_sort             = 1<<2, // -f disable sorting (enable -a)
	reverse_order       = 1<<3, // -r reverse sorting order
	long_format         = 1<<4, // -l show more info
	recusive            = 1<<5, // -R recusive files
	show_all            = 1<<6, // -a show dot files
	colors              = 1<<7, // -G enable coloring
	show_group_name     = 1<<8, // -g hide owner name (enable -l)
	only_current_dir    = 1<<9, // -d only show the current directory

}	t_flags;

typedef struct {
	t_flags	flags;
	t_flags	sorting;
	char	**files;
}	t_opts;

const t_flags	flags_map[256] = {
	['l'] = long_format,
	['R'] = recusive,
	['a'] = show_all,
	['G'] = colors,
	['g'] = long_format | show_group_name,
	['d'] = only_current_dir,

	['r'] = reverse_order,
	['f'] = show_all | no_sort,
	['t'] = time_modified_order,
	['u'] = time_access_order,
};
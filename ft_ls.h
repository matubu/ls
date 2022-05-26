#pragma once

#include "mem.h"
#include "files.h"
#include "sort.h"

#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>

typedef enum {
	sorting_flag        = 1,
	ascending_order     = 0, // default
	time_order          = 1, // -t sort by time modified

	use_access_time     = 1<<1, // -u sort by last access
	no_sort             = 1<<2, // -f disable sorting (enable -a)
	reverse_order       = 1<<3, // -r reverse sorting order
	nl_format           = 1<<4, // use nl instead of spaces
	long_format         = 1<<5, // -l show more info
	recusive            = 1<<6, // -R recusive files
	show_all            = 1<<7, // -a show dot files
	colors              = 1<<8, // -G enable coloring
	show_group_name     = 1<<9, // -g hide owner name (enable -l)
	only_current_dir    = 1<<10, // -d only show the current directory

}	t_flags;

typedef struct {
	t_flags	flags;
	t_flags	sorting;
	char	**files;
}	t_opts;

const t_flags	flags_map[256] = {
	['l'] = long_format | nl_format,
	['R'] = recusive,
	['a'] = show_all,
	['G'] = colors,
	['g'] = long_format | nl_format | show_group_name,
	['d'] = only_current_dir,

	['r'] = reverse_order,
	['f'] = show_all | no_sort,
	['t'] = time_order,
	['u'] = use_access_time,
};
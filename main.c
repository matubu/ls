#include "mem.h"
#include "files.h"
#include "sort.h"

#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>

void	usage(void) {
	puts("usage: ft_ls [-GRagdflrtu] [file ...]");
	exit(1);
}

void	addFlags(t_opts *opts, char *s)
{
	while (*s)
	{
		t_flags flag = flags_map[(unsigned int)*s];
		if (flag == 0)
		{
			put("ls: illegal option -- "); putch(*s); putch('\n');
			usage();
		}
		if (flag & sorting_flag)
			opts->sorting = flag & sorting_flag;
		opts->flags |= flag;
		++s;
	}
}

void	put_perms(mode_t mode)
{
	char perms[10] = "----------";

	if (S_ISDIR(mode))
		perms[0] = 'd';
	else if (S_ISLNK(mode))
		perms[0] = 'l';
	for (int i = 9, j = 1; (i -= 3) >= 0; j += 3)
	{
		if (mode & (1 << (2+i)))
			perms[j+0] = 'r';
		if (mode & (1 << (1+i)))
			perms[j+1] = 'w';
		if (mode & (1 << (0+i)))
			perms[j+2] = 'x';
	}
	write(1, perms, 10);
}

void	printFile(File *file, t_opts *opts, int fidx)
{
	if (!(opts->flags & nl_format) && fidx)
		putch(' ');

	if (opts->flags & long_format)
	{
		/* Perms */
		put_perms(file->stat.st_mode); putch(' ');
		/* Nb links */
		putun(file->stat.st_nlink); putch(' ');
		/* Owner */
		if (!(opts->flags & show_group_name))
		{ put(getpwuid(file->stat.st_uid)->pw_name); putch(' '); }
		/* Group */
		put(getgrgid(file->stat.st_gid)->gr_name); putch(' ');
		/* Size */
		putunp(file->stat.st_size); putch(' ');
		/* Time */
		char *tm = ctime(&file->time.tv_sec);
		write(1, tm, len(tm) - 1); putch(' ');
	}

	/* COLOR */
	if (opts->flags & colors)
	{
		if (S_ISDIR(file->stat.st_mode))
			put("\033[1;92m");
		else if (S_ISLNK(file->stat.st_mode))
			put("\033[95m");
		else if (file->stat.st_mode & S_IXUSR)
			put("\033[91m");
	}

	/* FILE */
	put(file->name);
	/* COLOR RESET */
	if (opts->flags & colors) put("\033[0m");

	/* LINK RESOLVE */
	if ((opts->flags & long_format) && S_ISLNK(file->stat.st_mode))
	{
		put(" -> ");
		char buf[1024];
		write(1, buf, readlink(file->path, buf, 1024));
	}

	if (opts->flags & nl_format)
		putch('\n');
}

void	printFileList(FileList *file_list, t_opts *opts)
{
	if (opts->flags & long_format)
	{
		int	total = 0;
		for (int i = 0; i < file_list->count; ++i)
			total += file_list->files[i].stat.st_blocks;
		put("total "); putun(total); putch('\n');
	}

	for (int i = 0; i < file_list->count; ++i)
	{
		File *file = (opts->flags & reverse_order) && !(opts->flags & no_sort)
			? file_list->files + file_list->count - i - 1
			: file_list->files + i;

		printFile(file, opts, i);
	}
}

const sorting_func_t	sorting_map[2] = {
	[ascending_order]     = sort_ascending,
	[time_order] = sort_modified
};

void	listFiles(File *file, t_opts *opts, int showpath)
{
	char *path = file->path;

	if (showpath)
	{ put(path); puts(":"); }
	DIR				*dir = opendir(path);

	if (dir == NULL)
	{
		put("ls: "); put(file->name); put(": "); put(strerror(errno)); putch('\n');
		return ;
	}
	FileList		file_list = { NULL, 0, 0 };
	struct dirent	*entry;

    while ((entry = readdir(dir)) != NULL)
		if (opts->flags & show_all || *entry->d_name != '.')
			pushFile(&file_list, joinpath(path, entry->d_name), entry->d_name, opts);

	closedir(dir);

	if (file_list.count == 0)
		goto clean;

	if (!(opts->flags & no_sort))
		sortFileList(&file_list, sorting_map[opts->sorting]);

	printFileList(&file_list, opts);

	if (!(opts->flags & nl_format))
		putch('\n');

	if (opts->flags & recusive)
		for (int i = 0; i < file_list.count; ++i)
			if (S_ISDIR(file_list.files[i].stat.st_mode)
				&& ft_strcmp(file_list.files[i].name, ".")
				&& ft_strcmp(file_list.files[i].name, ".."))
				{
					putch('\n');
					listFiles(file_list.files + i, opts, 1);
				}
clean:
	freeFileList(&file_list);
}

int	main(int ac, char **av)
{
	t_opts	opts = { 0, 0 };
	int		i = 1;

	if (ac == 0)
		usage();
	while (av[i] && av[i][0] == '-')
	{
		if (ft_strcmp(av[i], "--") == 0)
		{
			++i;
			break ;
		}
		addFlags(&opts, av[i++] + 1);
	}
	if (!isatty(1))
	{
		opts.flags &= ~colors;
		opts.flags |= nl_format;
	}

	FileList	files = { NULL, 0, 0 };

	if (i >= ac)
		pushFile(&files, ft_strdup("."), ".", &opts);

	for (char **file = av + i; *file; ++file)
		pushFile(&files, ft_strdup(*file), *file, &opts);

	if (!(opts.flags & no_sort))
		sortFileList(&files, sorting_map[opts.sorting]);

	int	fidx = 0;
	for (int j = 0; j < files.count; ++j)
		if (S_ISREG(files.files[j].stat.st_mode))
			printFile(files.files + j, &opts, fidx++);
	if (fidx && !(opts.flags & nl_format))
		putch('\n');

	for (int j = 0; j < files.count; ++j)
		if (S_ISDIR(files.files[j].stat.st_mode))
			listFiles(files.files + j, &opts, ac - i > 1);

	freeFileList(&files);
	return (0);
}
#include "ft_ls.h"

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
			printf("ls: illegal option -- %c\n", *s);
			usage();
		}
		if (flag & sorting_flag)
			opts->sorting = flag & sorting_flag;
		opts->flags |= flag;
		++s;
	}
}

void	pushFile(FileList *file_list, char *dir, char *name)
{
	struct stat buf;
	char *path = joinpath(dir, name);

	if (lstat(path, &buf) == -1)
	{
		printf("ls: %s: %s\n", name, strerror(errno));
		free(path);
		return ;
	}

	File	*tmp = ft_malloc(sizeof(File) * ++file_list->count);
	int i = file_list->count - 1;

	tmp[i].stat = buf;
	tmp[i].name = ft_strdup(name);
	tmp[i].path = path;

	while (i--)
		tmp[i] = file_list->files[i];

	free(file_list->files);
	file_list->files = tmp;
}

void	put_perms(mode_t mode)
{
	if (S_ISDIR(mode))
		putch('d');
	else if (S_ISLNK(mode))
		putch('l');
	else
		putch('-');
	for (int i = 3; i--;)
	{
		putch(mode & (1 << (2+i*3)) ? 'r' : '-');
		putch(mode & (1 << (1+i*3)) ? 'w' : '-');
		putch(mode & (1 << (0+i*3)) ? 'x' : '-');
	}
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

		if (opts->flags & long_format)
		{
			/* PERMS */
			put_perms(file->stat.st_mode); putch(' ');
			/* NB LINK */
			putun(file->stat.st_nlink); putch(' ');
			/* OWNER */
			if (!(opts->flags & show_group_name))
			{ put(getpwuid(file->stat.st_uid)->pw_name); putch(' '); }
			/* GROUP */
			put(getgrgid(file->stat.st_gid)->gr_name); putch(' ');
			/* SIZE */
			putunp(file->stat.st_size); putch(' ');
			/* LAST MODIF */
			char *tm = ctime(&file->stat.st_mtimespec.tv_sec);
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

		/* SEP */
		putch(((opts->flags & nl_format) || i == file_list->count - 1)
			? '\n'
			: ' ');
	}
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

const sorting_func_t	sorting_map[3] = {
	[ascending_order]     = sort_ascending,
	[time_modified_order] = sort_modified,
	[time_access_order]   = sort_access
};

void	listFiles(char *path, t_opts *opts)
{
	DIR				*dir = opendir(path);

	if (dir == NULL)
	{
		printf("ls: %s: %s\n", path, strerror(errno));
		return ;
	}
	FileList		file_list = { NULL, 0 };
	struct dirent	*entry;

    while ((entry = readdir(dir)) != NULL)
		if (opts->flags & show_all || *entry->d_name != '.')
			pushFile(&file_list, path, entry->d_name);

	closedir(dir);

	if (file_list.count == 0)
		goto clean;

	if (!(opts->flags & no_sort))
		sortFileList(&file_list, sorting_map[opts->sorting]);

	printFileList(&file_list, opts);

	if (opts->flags & recusive)
	{
		for (int i = 0; i < file_list.count; ++i)
		{
			if (S_ISDIR(file_list.files[i].stat.st_mode)
				&& ft_strcmp(file_list.files[i].name, ".")
				&& ft_strcmp(file_list.files[i].name, ".."))
			{
				putch('\n'); put(file_list.files[i].path); puts(":");
				listFiles(file_list.files[i].path, opts);
			}
		}
	}

	clean:
	freeFileList(&file_list);
}

void	list(t_opts *opts)
{
	char	**files = opts->files;

	if (*files)
	{
		do {
			listFiles(*files, opts);
		} while (*++files);
	}
	else
	{
		listFiles(".", opts);
	}
}

// TODO long format
// TODO remove printf
int	main(int ac, char **av)
{
	t_opts	opts = { 0, 0, NULL };
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
	opts.files = av + i;
	if (!isatty(1))
	{
		opts.flags &= ~colors;
		opts.flags |= nl_format;
	}
	list(&opts);
	system("leaks ft_ls | grep leaked");
}
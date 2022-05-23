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

void	printFileList(FileList *file_list, t_opts *opts)
{
	for (int i = 0; i < file_list->count; ++i)
	{
		File *file = file_list->files + i;

		if (opts->flags & colors)
		{
			if (S_ISDIR(file->stat.st_mode))
				put("\033[1;92m");
			else if (S_ISLNK(file->stat.st_mode))
				put("\033[95m");
			else if (file->stat.st_mode & S_IXUSR)
				put("\033[91m");
		}

		if ((opts->flags & reverse_order) && !(opts->flags & no_sort))
			file = file_list->files + file_list->count - i - 1;

		put(file->name);
		put("\033[0m");

		putch(((opts->flags & long_format) || i == file_list->count - 1)
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

// ls -Ra ../
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
	list(&opts);
}
#include "ft_ls.h"

void	usage(void) {
	puts("usage: ft_ls [-GRagdflrtu] [file ...]");
	exit(1);
}

void	allocationError(void) {
	puts("allocation error: cannot allocate");
	exit(1);
}

void	addFlags(t_opts *opts, char *s)
{
	while (*s)
	{
		t_flags	flag = flags_map[(unsigned int)*s];
		if (flag == 0)
		{
			printf("ls: illegal option -- %c\n", *s);
			usage();
		}
		opts->flags |= flag;
		++s;
	}
}

void	checkFiles(t_opts *opts)
{
	struct stat buf;

	for (int i = 0; opts->files[i]; ++i)
		if (stat(opts->files[i], &buf) == -1)
			printf("ls: %s: %s\n", opts->files[i], strerror(errno));
}

typedef struct {
	char	**files;
	int		count;
}	FileList;

void	pushFile(FileList *file_list, char *s)
{
	file_list->files = realloc(
		file_list->files,
		sizeof(char *) * ++file_list->count
	);
	char	*file = strdup(s);
	if (file_list->files == NULL || file == NULL)
		allocationError();
	file_list->files[file_list->count - 1] = file;
}

void	printFileList(FileList *file_list, t_opts *opts)
{
	for (int i = 0; i < file_list->count; ++i)
	{
		printf("%-40s", file_list->files[i]);
		if (opts->flags & long_format)
			putchar('\n');
	}
	putchar('\n');
}

void	freeFileList(FileList *file_list)
{
	while (file_list->count--)
		free(file_list->files[file_list->count]);
	free(file_list->files);
	file_list->files = NULL;
}

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
		pushFile(&file_list, entry->d_name);

	// sort()

	printFileList(&file_list, opts);

	freeFileList(&file_list);

    closedir(dir);
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

int	main(int ac, char **av)
{
	t_opts	opts = { 0, NULL };
	int		i = 1;

	if (ac == 0)
		usage();
	while (av[i] && av[i][0] == '-')
		addFlags(&opts, av[i++] + 1);
	opts.files = av + i;
	checkFiles(&opts);
	list(&opts);
}
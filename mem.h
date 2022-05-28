#pragma once

#include "utils.h"
#include <stdlib.h>


// #include <stdlib.h>
// #include <stdio.h>

// typedef struct __s_malloc_node {
// 	void					*ptr;
// 	size_t					size;
// 	char					*file;
// 	int						line;
// 	int						freed;
// 	char					*free_file;
// 	int						free_line;
// 	struct __s_malloc_node	*next;
// }	__t_malloc_node;

// __t_malloc_node	*__malloc_list = NULL;

// void	__check_leaks()
// {
// 	int	leaks = 0;
// 	int	allocations = 0;
// 	while (__malloc_list)
// 	{
// 		if (!__malloc_list->freed)
// 		{
// 			fprintf(stderr, "%s:%d: \033[95;1mleak:\033[0m %zu bytes leaked\n",
// 				__malloc_list->file,
// 				__malloc_list->line,
// 				__malloc_list->size
// 			);
// 			++leaks;
// 		}
// 		__malloc_list = __malloc_list->next;
// 		++allocations;
// 	}
// 	fprintf(stderr, "\033[91m%d leaks\033[0m, %d allocations\n", leaks, allocations);
// }

// void	*__safe_malloc(size_t size, char *file, int line)
// {
// 	void	*ptr = malloc(size);

// 	if (ptr == NULL)
// 	{
// 		fprintf(stderr, "%s:%d: \033[91;1mmalloc:\033[0m cannot allocate %zu bytes\n", file, line, size);
// 		abort();
// 	}
// 	if (__malloc_list == NULL)
// 		atexit(__check_leaks);
// 	__t_malloc_node	*malloc_node = malloc(sizeof(__t_malloc_node));
// 	malloc_node->ptr = ptr;
// 	malloc_node->size = size;
// 	malloc_node->file = file;
// 	malloc_node->line = line;
// 	malloc_node->freed = 0;
// 	malloc_node->next = __malloc_list;
// 	__malloc_list = malloc_node;
// 	return (ptr);
// }

// void	__safe_free(void *ptr, char *file, int line)
// {
// 	__t_malloc_node	**list = &__malloc_list;

// 	while (*list)
// 	{
// 		if ((*list)->ptr == ptr)
// 		{
// 			if ((*list)->freed)
// 			{
// 				fprintf(stderr, "%s:%d: \033[91;1mfree:\033[0m %p\n | already freed at %s:%d\n | mallocated at %s:%d\n",
// 					file, line, ptr,
// 					(*list)->free_file, (*list)->free_line,
// 					(*list)->file, (*list)->line
// 				);
// 				abort();
// 			}
// 			free(ptr);
// 			(*list)->freed = 1;
// 			(*list)->free_file = file;
// 			(*list)->free_line = line;
// 			return ;
// 		}
// 		list = &(*list)->next;
// 	}
// 	fprintf(stderr, "%s:%d: \033[%d;1mfree:\033[0m %p was not allocated\n", file, line, ptr ? 91 : 93, ptr);
// 	if (ptr)
// 		abort();
// }

// #define malloc(n) __safe_malloc(n, __FILE__, __LINE__)
// #define free(ptr) __safe_free(ptr, __FILE__, __LINE__)


























void	*ft_malloc(size_t size)
{
	void	*new = malloc(size);

	if (new == NULL)
	{
		puts("ls: allocation error");
		exit(1);
	}
	return (new);
}

char	*ft_strdup(const char *s)
{
	char	*new = ft_malloc(len(s) + 1);
	char	*ptr = new;
	while ((*ptr++ = *s++));
	return (new);
}
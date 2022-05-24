NAME = ft_ls

SRCS = $(wildcard *.c)
DEPS = $(wildcard *.h) Makefile
OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror -Ofast# -fsanitize=address -g

all: $(NAME)

%.o: %.c $(DEPS)
	gcc $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

test: all
	ls $(ARGS)
	ls $(ARGS) | cat > .ls.out
	ls $(ARGS)
	./ft_ls -l $(ARGS) > .ft_ls.out
	diff .ls.out .ft_ls.out

.PHONY: all clean fclean re
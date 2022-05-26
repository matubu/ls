NAME = ft_ls

SRCS = $(wildcard *.c)
DEPS = $(wildcard *.h) Makefile
OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror -Ofast

all: $(NAME)

bonus: all

%.o: %.c $(DEPS)
	gcc $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re
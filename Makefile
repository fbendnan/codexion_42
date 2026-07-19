NAME        = codexion
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread -fsanitize=address -g
SRCS     = coders/parsing.c coders/utils.c coders/dongle.c coders/codexion.c coders/coder.c coders/scheduler.c \
			coders/monitor.c coders/init.c coders/main.c coders/cleanup.c
OBJS     = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o : %.c codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
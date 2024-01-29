NAME			:= philo

CC				:= cc
CFLAGS			:= -g -fsanitize=thread -pthread -Wall -Wextra -Werror
RM				:= rm -rf

HEADERS_DIR		:= ./includes/
SRCS_PATH		:= ./srcs/

SRCS_CORE		:= main.c libft.c \
	reader.c cleaner.c \
	initialization.c philosophers.c \
	wrappers.c checkers.c \
	printing.c procedures.c \
	threads_utils.c

HEADERS			:= -I$(HEADERS_DIR)
HEADERS_CORE 	:= $(HEADERS_DIR)/philosophers.h

OBJS_CORE		:= $(SRCS_CORE:.c=.o)

OBJS			:= $(addprefix $(SRCS_PATH), $(OBJS_CORE))
SRCS			:= $(addprefix $(SRCS_PATH), $(SRCS_CORE))

GREEN			:= \033[0;32m
BLUE			:= \033[0;34m
NC				:= \033[0m

all: $(NAME)

$(NAME): $(SRCS) $(OBJS) $(HEADERS_CORE)
	@$(CC) $(CFLAGS) $(HEADERS) $(SRCS) -o $(NAME)
	@echo "$(GREEN)--- Compilation of '$(NAME)' completed successfully ---$(NC)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(NAME) $(NAME).dSYM .DS_Store
	@$(RM) $(OBJS) $(OBJS_BONUS)
	@echo "$(BLUE)--- Object files cleaned ---$(NC)"

fclean: clean
	@echo "$(BLUE)--- '$(NAME)' removed ---$(NC)"
	@echo "$(BLUE)--- Cleaning finished ---$(NC)"

re: fclean all
	@echo "$(GREEN)--- Everything recompiled ---$(NC)"

.PHONY: all clean fclean re
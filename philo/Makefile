NAME			:= philo

CC				:= cc
CFLAGS			:= -g -Wall -Wextra -Werror
RM				:= rm -rf

HEADERS_DIR		:= ./includes/
SRCS_PATH		:= ./srcs/

SRCS_CORE		:= main.c utils.c reader.c errors.c initialization.c data_init.c philosophers.c wrappers.c
# SRCS_BONUS		:= 

HEADERS			:= -I$(HEADERS_DIR)
HEADERS_CORE 	:= $(HEADERS_DIR)/philosophers.h
# HEADERS_BONUS 	:=

OBJS_CORE		:= $(SRCS_CORE:.c=.o)
# OBJS_BONUS		:= $(SRCS_BONUS:.c=.o)

OBJS			:= $(addprefix $(SRCS_PATH), $(OBJS_CORE))
# OBJS_BONUS		:= $(addprefix $(SRCS_PATH), $(OBJS_BONUS))
SRCS			:= $(addprefix $(SRCS_PATH), $(SRCS_CORE))
# SRCS_BONUS		:= $(addprefix $(SRCS_PATH), $(SRCS_BONUS))

GREEN			:= \033[0;32m
BLUE			:= \033[0;34m
NC				:= \033[0m

all: $(NAME)

$(NAME): $(SRCS) $(OBJS) $(HEADERS_CORE)
	@$(CC) $(CFLAGS) $(HEADERS) $(SRCS) -o $(NAME)
	@echo "$(GREEN)--- Compilation of '$(NAME)' completed successfully ---$(NC)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

# bonus: .bonus

# .bonus: $(SRCS_BONUS) $(OBJS_BONUS) $(HEADERS_BONUS)
# 	@$(CC) $(CFLAGS) $(HEADERS) $(SRCS_BONUS) -o $(NAME)
# 	@touch .bonus
# 	@echo "$(GREEN)--- Compilation of '$(NAME)' completed successfully ---$(NC)"

clean:
	@$(RM) $(NAME)
	@$(RM) $(NAME).dSYM
	@$(RM) $(OBJS) $(OBJS_BONUS)
	@echo "$(BLUE)--- Object files cleaned ---$(NC)"

fclean: clean
	@echo "$(BLUE)--- '$(NAME)' removed ---$(NC)"
	@echo "$(BLUE)--- Cleaning finished ---$(NC)"

re: fclean all
	@echo "$(GREEN)--- Everything recompiled ---$(NC)"

.PHONY: all clean fclean re
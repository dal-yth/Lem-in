NAME = lem-in

CC = gcc

FLAGS = -Wall -Wextra -Werror

DIR_S = sources

DIR_O = obj

SOURCES = main.c free.c data.c input.c utils.c algo_utils.c dinics_algo.c \
		get_paths.c input_utils.c buckets.c move_ants.c path_funcs.c \
		antprint.c print_funcs.c brute.c brute_combis.c brute_utils.c \
		data_utils.c 

LIBFT = libft/libft.a

INC = includes/

SRCS = $(addprefix $(DIR_S)/,$(SOURCES))

OBJ = $(addprefix $(DIR_O)/,$(SOURCES:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Compiling library..."
	@cd libft && make
	@echo "Creating executable..."
	@$(CC) -o $(NAME) $(FLAGS) $(OBJ) $(LIBFT)
	@echo "$(NAME) created."

$(DIR_O)/%.o: $(DIR_S)/%.c
	@mkdir -p $(DIR_O)
	@$(CC) $(FLAGS) -I $(INC) -o $@ -c $<

clean:
	@echo "Removing libft objects..."
	@cd libft && make clean
	@echo "Removing project objects..."
	@rm -f $(OBJ)
	@echo "Removing object directory..."
	@rm -rf $(DIR_O)

fclean: clean
	@echo "Removing libft.a..."
	@cd libft && make fclean
	@echo "Removing executable..."
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

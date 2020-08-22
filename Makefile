NAME = lem-in

CC = gcc

FLAGS = -Wall -Wextra -Werror

MKDIR_P = mkdir -p

DIR_S = sources

DIR_O = obj

SOURCES = main.c free.c data.c input.c utils.c algo_utils.c dinics_algo.c \
		get_paths.c input_utils.c buckets.c move_ants.c path_funcs.c \
		antprint.c print_funcs.c brute.c brute_combis.c brute_utils.c \
		data_utils.c 

LIBFT = libft/libft.a

INC = includes/

OBJ = $(addprefix $(DIR_O)/,$(SOURCES:.c=.o))

PROGRESS_CHAR = "\e[92m█\e[0m"

all: dirs $(NAME)

$(NAME): $(OBJ)
	@echo "\nCompiling library..."
	@cd libft && make
	@echo "Creating executable..."
	@$(CC) -o $(NAME) $(FLAGS) $(OBJ) $(LIBFT)
	@echo "\e[1m$(NAME) created!\e[0m"

dirs:
	@echo "Creating object dir..."
	@${MKDIR_P} ${DIR_O}

$(DIR_O)/%.o: $(DIR_S)/%.c
	@$(CC) $(FLAGS) -I $(INC) -o $@ -c $<
	@echo $(PROGRESS_CHAR)"\c"

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

.PHONY: all clean fclean re dirs

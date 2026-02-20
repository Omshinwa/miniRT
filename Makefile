NAME = miniRT

SRC =	main.c \
		app/app.c \
		app/hooks.c \
		draw/draw.c \
		math/math.c \
		raytrace/raytrace.c \
        env3d/camera.c

OBJ = $(SRC:.c=.o)

SRC_DIR = src
OBJ_DIR = build

SOURCE_FILES = $(addprefix $(SRC_DIR)/, $(SRC))
OBJECT_FILES = $(addprefix $(OBJ_DIR)/, $(OBJ))
DEPENDANCIES = $(OBJECT_FILES:.o=.d)

CMPL_CMD = cc -Wall -Wextra -Werror -g3 -MMD -MP #-Ofast -march=native -mtune=native -ffast-math -funroll-loops
LINK_CMD = cc -lm -lXext -lX11

# library dependencies
LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a
MINILIBX = minilibx-linux
MINILIBX_A = $(MINILIBX)/libmlx.a

# Rules
all: $(NAME)

# linking
$(NAME): $(OBJECT_FILES) Makefile $(LIBFT_A) $(MINILIBX_A)
	$(LINK_CMD) $(OBJECT_FILES) $(LIBFT_A) $(MINILIBX_A) -o $(NAME)

# compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CMPL_CMD) -c $< -o $@

-include $(DEPENDANCIES)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(MINILIBX) clean

fclean: clean
	rm -rf $(NAME) $(NAME)_bonus
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(MINILIBX) clean

re: fclean all

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)
$(MINILIBX_A):
	$(MAKE) -C $(MINILIBX)

.PHONY: all clean fclean re

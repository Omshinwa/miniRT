# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/09 10:48:13 by dasamuel          #+#    #+#              #
#    Updated: 2026/03/18 08:51:42 by dasamuel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

PURPLE		:=	$(shell printf '\033[1;35m')
RED			:=	$(shell printf '\033[1;31m')
GREEN		:=	$(shell printf '\033[1;32m')
BLUE		:=	$(shell printf '\033[1;34m')
RESET		:=	$(shell printf '\033[0m')

TARGET		=	miniRT

RM			=	rm -rf

SRCS_DIR	=	src

OBJS_DIR	=	build

SRCS		=	main.c						\
				app/app.c					\
				app/key_hook.c				\
				app/mouse_hook.c			\
				app/hooks.c					\
				app/window.c				\
				app/draw_info.c				\
				app/camera.c				\
				parse/get_next_line.c		\
				parse/parse_0_file.c		\
				parse/parse_1_line.c		\
				parse/parse_2_instruction.c	\
				parse/parse_3_field.c		\
				parse/parse_4_token.c		\
				render/lighting.c			\
				render/render.c				\
				render/shadow.c				\
				render/bump_map.c			\
				render/threaded_render.c	\
				render/bump_tbn.c			\
				math/math.c					\
				math/vec3_basic.c			\
				math/vec3_ops.c				\
				math/vec3_utils.c			\
				raytrace/obj_cone.c			\
				raytrace/obj_cone2.c		\
				raytrace/obj_cylinder.c		\
				raytrace/obj_cylinder2.c	\
				raytrace/obj_plane.c		\
				raytrace/obj_sphere.c		\
				raytrace/raytrace.c

OBJS	=	$(SRCS:.c=.o)

SOURCE_FILES	=	$(addprefix $(SRCS_DIR)/, $(SRCS))
OBJECT_FILES	=	$(addprefix $(OBJS_DIR)/, $(OBJS))
DEPENDANCIES	=	$(OBJECT_FILES:.o=.d)

CPPFLAGS	=
CFLAGS		=	-Wall -Wextra -Werror -g3 -pthread -std=gnu17
CMPL_CMD	=	cc $(CFLAGS) -MMD -MP $(CPPFLAGS)
LINK_CMD	=	cc -pthread -lm -lXext -lX11

# library dependencies
LIBFT_DIR	=	libft
LIBFT_A		=	$(LIBFT_DIR)/libft.a
MINILIBX	=	minilibx-linux
MINILIBX_A	=	$(MINILIBX)/libmlx_Linux.a

# Rules
all: $(TARGET)


# linking
$(TARGET): $(OBJECT_FILES) Makefile $(LIBFT_A) $(MINILIBX_A)
	@$(LINK_CMD) $(OBJECT_FILES) $(LIBFT_A) $(MINILIBX_A) -o $(TARGET)
	@echo "$(PURPLE)"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "  ██████╗  █████╗ ██╗   ██╗████████╗██████╗  █████╗  ██████╗██╗███╗   ██╗ ██████╗ "
	@echo "  ██╔══██╗██╔══██╗╚██╗ ██╔╝╚══██╔══╝██╔══██╗██╔══██╗██╔════╝██║████╗  ██║██╔════╝ "
	@echo "  ██████╔╝███████║ ╚████╔╝    ██║   ██████╔╝███████║██║     ██║██╔██╗ ██║██║  ███╗"
	@echo "  ██╔══██╗██╔══██║  ╚██╔╝     ██║   ██╔══██╗██╔══██║██║     ██║██║╚██╗██║██║   ██║"
	@echo "  ██║  ██║██║  ██║   ██║      ██║   ██║  ██║██║  ██║╚██████╗██║██║ ╚████║╚██████╔╝"
	@echo "  ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝      ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝╚═╝  ╚═══╝ ╚═════╝ "
	@echo ""
	@echo "$(RED)                · ─────────────────────────────► ●  sphere"
	@echo "              ·  $(RESET)"
	@echo "       👁️  ·$(GREEN)  ·  ·  · ──────────────────────────────► ▭  plane"
	@echo "$(BLUE)              ·  "
	@echo "                · ─────────────────────────────► △  cone$(PURPLE)"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "$(GREEN)is ready 🎉🥳🎊"
	@echo "$(GREEN)Made by$(RESET) $(RED)wiwu $(GREEN)and $(RED)dasamuel$(GREEN).$(RESET)"

# compilation
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CMPL_CMD) -c $< -o $@

-include $(DEPENDANCIES)

clean:
	@$(RM) $(OBJS_DIR)
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@$(MAKE) -C $(MINILIBX) clean  > /dev/null 2>&1
	@echo "$(GREEN) Objects files successfully delete 🎉$(RESET)"

fclean: clean
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@echo "$(GREEN) Executable files successfully delete 🎉$(RESET)"

re: fclean all

$(LIBFT_A):
	@$(MAKE) -s -C $(LIBFT_DIR)
$(MINILIBX_A):
	@$(MAKE) -C $(MINILIBX) > /dev/null 2>&1

.PHONY: all clean fclean re

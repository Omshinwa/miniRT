/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_0_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:17:11 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/16 08:25:38 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../app/app.h"
#include "parse_private.h"

static const int	ALL_TOGGLE = 2147483647;
static const char	*C_RED = "\e[0;31m";
static const char	*C_YELLOW = "\e[0;33m";
static const char	*C_CYAN = "\e[0;36m";
static const char	*C_RESET = "\e[0m";

static void	print_err_set_vars(const bool should_keep_going, const char **color,
									int *fd, bool *first_time)
{
	if (should_keep_going)
	{
		*color = C_YELLOW;
		*fd = STDOUT_FILENO;
	}
	else
	{
		*color = C_RED;
		*fd = STDERR_FILENO;
		if (!*first_time)
		{
			*first_time = true;
			ft_putstr_fd("Error\n", *fd);
		}
	}
}

bool	print_err(const char *msg, const char *wrong_data
	, const bool should_keep_going)
{
	const char	*color;
	int			fd;
	static bool	first_time;

	print_err_set_vars(should_keep_going, &color, &fd, &first_time);
	ft_putstr_fd(color, fd);
	while (*msg && *msg != '@')
		ft_putchar_fd(*msg++, fd);
	if (*msg)
	{
		msg++;
		ft_putstr_fd(C_CYAN, fd);
		ft_putchar_fd('`', fd);
		while (*wrong_data)
			ft_putchar_fd(*wrong_data++, fd);
		ft_putchar_fd('`', fd);
		ft_putstr_fd(color, fd);
		while (*msg)
			ft_putchar_fd(*msg++, fd);
	}
	ft_putstr_fd(C_RESET, fd);
	return (should_keep_going);
}

static bool	check_extension(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 4)
		return (0);
	if (ft_strncmp(filename + len - 3, ".rt", 3) != 0)
		return (print_err("Invalid file extension (expected @) \n",
				".rt", false));
	return (true);
}

static bool	parse_scene_file(t_app *app, char *filename)
{
	int		fd;
	char	*line;

	if (!check_extension(filename))
		return (false);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (perror(filename), false);
	line = get_next_line_strip_nl(fd);
	while (line)
	{
		if (!parse_line(line, app))
		{
			print_err("failed to parse line: @\n", line, false);
			return (free(line), close(fd), false);
		}
		free(line);
		line = get_next_line_strip_nl(fd);
	}
	close(fd);
	return (true);
}

int	setup_scene(t_app *app, char *filename)
{
	app->scene = safe_calloc(1, sizeof(t_scene));
	if (!app->scene)
		exit_n_clean(app, RED "failed to create scene" RESET);
	app->scene->camera.pos = (t_vec3){0, 0, 0};
	app->scene->camera.right = (t_vec3){1, 0, 0};
	app->scene->camera.up = (t_vec3){0, 1, 0};
	app->scene->camera.forward = (t_vec3){0, 0, 1};
	app->scene->camera.fov = 70;
	if (!parse_scene_file(app, filename))
		exit_n_clean(app, RED "failed to parse scene" RESET);
	app->scene->render_mode = ALL_TOGGLE;
	return (0);
}

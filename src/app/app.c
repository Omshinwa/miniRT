/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:14:45 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/18 06:35:02 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"

// Free MLX display and scene data (objects, lights)
void	clean_scene(t_app *app)
{
	if (app->mlx)
	{
		mlx_destroy_display(app->mlx);
		free(app->mlx);
	}
	if (app->scene)
	{
		if (app->scene->objects)
			free(app->scene->objects);
		if (app->scene->lights)
			free(app->scene->lights);
		free(app->scene);
	}
}

// Destroy all MLX resources (window, images, textures, bump maps) then exit
void	exit_n_clean(t_app *app, char *msg)
{
	int	i;

	if (msg)
		ft_putendl_fd(msg, 2);
	if (!app)
		exit(1);
	if (app->mlx && app->win)
		mlx_destroy_window(app->mlx, app->win);
	if (app->mlx && app->img.mlx_img)
		mlx_destroy_image(app->mlx, app->img.mlx_img);
	i = -1;
	while (app->scene && ++i < app->scene->obj_count)
	{
		if (app->scene->objects[i].texture.mlx_img)
			mlx_destroy_image(app->mlx, app->scene->objects[i].texture.mlx_img);
		if (app->scene->objects[i].bump_map.mlx_img)
			mlx_destroy_image(app->mlx,
				app->scene->objects[i].bump_map.mlx_img);
	}
	clean_scene(app);
	free(app);
	if (msg)
		exit(1);
	exit(0);
}

// Wrapper around ft_calloc, logs to stderr on failure. Caller handles NULL
void	*safe_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	ptr = ft_calloc(nmemb, size);
	if (!ptr)
		ft_putendl_fd(RED "Error: calloc failed" RESET, 2);
	return (ptr);
}

// Allocate app, init MLX window/image, setup hooks. Exits on failure
t_app	*create_app(void)
{
	t_app	*app;

	app = safe_calloc(1, sizeof(t_app));
	if (!app)
		return (NULL);
	app->mlx = mlx_init();
	if (!app->mlx)
		exit_n_clean(app, RED "Failed to init mlx" RESET);
	app->win = mlx_new_window(app->mlx, WINDOW_X, WINDOW_Y, "miniRT");
	if (!app->win)
		exit_n_clean(app, RED "Failed to create window" RESET);
	app->img.mlx_img = mlx_new_image(app->mlx, WINDOW_X, WINDOW_Y);
	if (!app->img.mlx_img)
		exit_n_clean(app, RED "Failed to load image" RESET);
	if (gettimeofday(&app->time, NULL) == -1)
		exit_n_clean(app, RED "Failed to get time of day" RESET);
	app->img.first_pixel = mlx_get_data_addr(app->img.mlx_img,
			&app->img.bits_per_pixel, &app->img.size_line, &app->img.endian);
	if (!app->img.first_pixel)
		exit_n_clean(app, RED "Failed to get address of first pixel" RESET);
	app->img.width = WINDOW_X;
	app->img.height = WINDOW_Y;
	hook_everything(app);
	return (app);
}

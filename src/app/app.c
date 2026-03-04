#include "../main.h"
#include "app.h"

//    █████████   ███████████  ███████████
//   ███░░░░░███ ░░███░░░░░███░░███░░░░░███
//  ░███    ░███  ░███    ░███ ░███    ░███
//  ░███████████  ░██████████  ░██████████
//  ░███░░░░░███  ░███░░░░░░   ░███░░░░░░
//  ░███    ░███  ░███         ░███
//  █████   █████ █████        █████
// ░░░░░   ░░░░░ ░░░░░        ░░░░░

// Whenever a malloc fail, we exit and free everything
int exit_n_clean(t_app *app)
{
	if (!app)
		return (0);
	if (app->mlx && app->win)
		mlx_destroy_window(app->mlx, app->win);
	if (app->mlx && app->img.mlx_img)
		mlx_destroy_image(app->mlx, app->img.mlx_img);
	if (app->mlx)
	{
		mlx_destroy_display(app->mlx);
		free(app->mlx);
	}
	if (app->scene)
	{
		if (app->scene->objects)
			free(app->scene->objects);
		free(app->scene);
	}
	free(app);
	exit(0);
	return (0);
}

// Create the window
t_app *create_app(void)
{
	t_app *app;

	app = ft_calloc(1, sizeof(t_app));
	if (!app)
		return (NULL);
	app->mlx = mlx_init();
	if (!app->mlx)
		exit_n_clean(app);
	app->win = mlx_new_window(app->mlx, WINDOW_X, WINDOW_Y, "YAAAAAAY");
	app->img.mlx_img = mlx_new_image(app->mlx, WINDOW_X, WINDOW_Y);
	if (!app->img.mlx_img || !app->win || gettimeofday(&app->time, NULL) == -1)
		exit_n_clean(app);
	app->first_pixel = mlx_get_data_addr(app->img.mlx_img, &app->bits_per_pixel,
										 &app->size_line, &app->endian);
	if (!app->first_pixel)
		exit_n_clean(app);

	hook_everything(app);
	return (app);
}
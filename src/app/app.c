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
	if (app->mlx_ptr && app->win_ptr)
		mlx_destroy_window(app->mlx_ptr, app->win_ptr);
	if (app->mlx_ptr && app->img_ptr)
		mlx_destroy_image(app->mlx_ptr, app->img_ptr);
	if (app->mlx_ptr)
	{
		mlx_destroy_display(app->mlx_ptr);
		free(app->mlx_ptr);
	}
	free(app);
	exit(0);
	return (0);
}

// Create the window
t_app *create_app(void)
{
	t_app *app;
	char *p;
	char *(*f)(void *, int *, int *, int *);

	app = malloc(sizeof(t_app));
	if (!app)
		return (NULL);
	app->mlx_ptr = mlx_init();
	if (!app->mlx_ptr)
		exit_n_clean(app);
	app->win_ptr = mlx_new_window(app->mlx_ptr, WINDOW_X, WINDOW_Y, "YAAAAAAY");
	app->img_ptr = mlx_new_image(app->mlx_ptr, WINDOW_X, WINDOW_Y);
	if (!app->img_ptr || !app->win_ptr)
		exit_n_clean(app);
	if (gettimeofday(&app->time, NULL) == -1)
		exit_n_clean(app);
	f = mlx_get_data_addr;
	p = f(app->img_ptr, &app->pixel_depth, &app->size_line, &app->endian);
	app->first_pixel = p;
	if (!app->first_pixel)
		exit_n_clean(app);

	// init objects
	app->global_cam = (t_camera){{0, 0, 0}, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, 70};
	return (app);
}
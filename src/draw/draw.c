#include "../main.h"
#include "draw.h"

//  ██████████   ███████████     █████████   █████   ███   █████
// ░░███░░░░███ ░░███░░░░░███   ███░░░░░███ ░░███   ░███  ░░███
//  ░███   ░░███ ░███    ░███  ░███    ░███  ░███   ░███   ░███
//  ░███    ░███ ░██████████   ░███████████  ░███   ░███   ░███
//  ░███    ░███ ░███░░░░░███  ░███░░░░░███  ░░███  █████  ███
//  ░███    ███  ░███    ░███  ░███    ░███   ░░░█████░█████░
//  ██████████   █████   █████ █████   █████    ░░███ ░░███
// ░░░░░░░░░░   ░░░░░   ░░░░░ ░░░░░   ░░░░░      ░░░   ░░░

// Change the memory value in the image
static void set_image_pixel_at(t_app *app, int x, int y, int color)
{
	int *buffer;
	int bytes_per_pixel;

	bytes_per_pixel = app->pixel_depth / 8;
	buffer = (int *)app->first_pixel;
	buffer[(y * app->size_line / bytes_per_pixel) + x] = color;
}

// redraw the image
void redraw(t_app *app)
{
	int x;
	int y;
	int color;

	x = -1;
	while (++x < WINDOW_X)
	{
		y = -1;
		while (++y < WINDOW_Y)
		{
			color = calc_raytrace(app->env3d, x, y);
			set_image_pixel_at(app, x, y, color);
		}
	}
	mlx_put_image_to_window(app->mlx_ptr, app->win_ptr, app->img_ptr, 0, 0);
}
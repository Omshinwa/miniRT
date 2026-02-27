#include "render.h"
#include "../main.h"
#include "../render/render.h"
#include "../../minilibx-linux/mlx.h"
#include "../app/app.h"

//  ██████████   ███████████     █████████   █████   ███   █████
// ░░███░░░░███ ░░███░░░░░███   ███░░░░░███ ░░███   ░███  ░░███
//  ░███   ░░███ ░███    ░███  ░███    ░███  ░███   ░███   ░███
//  ░███    ░███ ░██████████   ░███████████  ░███   ░███   ░███
//  ░███    ░███ ░███░░░░░███  ░███░░░░░███  ░░███  █████  ███
//  ░███    ███  ░███    ░███  ░███    ░███   ░░░█████░█████░
//  ██████████   █████   █████ █████   █████    ░░███ ░░███
// ░░░░░░░░░░   ░░░░░   ░░░░░ ░░░░░   ░░░░░      ░░░   ░░░

// P is a point on the surface of the OBJ
// we apply the OBJ's color
// the texture
// the checkerboard
// the bumpmap?
t_vec3 compute_obj_material(t_object *obj, t_vec3 P)
{
	t_vec3 color;
	float uv[2];

	// color = (t_vec3){1, 1, 1};
	color = obj->color;
	if (obj->type == OBJ_SPHERE)
		sphere_uv(P, obj->data.sphere, &uv[0], &uv[1]);
	else if (obj->type == OBJ_PLANE)
		plane_uv(P, obj->data.plane, &uv[0], &uv[1]);
	else
		assert(0);

	// texture

	// luminosity

	// checkerboard
	if (obj->checker)
	{
		int u = floor(uv[0] * g_CHECKERBOARD_SCALE_FACTOR);
		int v = floor(uv[1] * g_CHECKERBOARD_SCALE_FACTOR);
		if ((u + v) % 2 != 0)
			color = (t_vec3){0, 0, 0};
	}
	return (color);
}

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
			color = calc_pixel_color(app->scene, x, y);
			set_image_pixel_at(app, x, y, color);
		}
	}
	mlx_put_image_to_window(app->mlx_ptr, app->win_ptr, app->img_ptr, 0, 0);
}

// Given a x,y pixel coordinate, calculate its color
int calc_pixel_color(t_scene *scene, int x, int y)
{
	t_vec3 d_vector;
	t_vec3 color;
	t_object *obj;
	float dist;

	d_vector = camera_pixel_to_vector(scene->global_cam, x, y);
	obj = get_hit(scene, scene->global_cam.pos, d_vector, &dist);
	color = (t_vec3){0, 0, 0};

	if (obj) // we hit something
	{
		float clamp;
		clamp = fmaxf(0, 255.0f - dist);
		color = (t_vec3){clamp / 255.0f, clamp / 255.0f, clamp / 255.0f}; // gray

		// calc UV and do diverse stuff
		// // object checkerboard pattern
		t_vec3 P = t_to_P(scene->global_cam.pos, d_vector, dist);
		color = color_mult(color, compute_obj_material(obj, P));
	}
	// if ambient light
	color = vec3_add(color, vec3_scale(scene->ambient_light.color, scene->ambient_light.brightness));
	return to_color_int(color);
}
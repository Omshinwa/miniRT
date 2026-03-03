#include "render.h"
#include "../main.h"
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

static t_vec3 int_to_color(int input)
{
	t_vec3 color;

	float R = (input >> 16 & 0x0000FF);
	float G = (input >> 8 & 0x0000FF);
	float B = (input & 0x0000FF);
	color = (t_vec3){R / 255.0f, G / 255.0f, B / 255.0f};
	return (color);
}

static t_vec3 get_texture_pixel_at(t_mlx_img img, float u, float v)
{
	int color;
	int *buffer;
	int x;
	int y;
	x = floor(u * img.width);
	y = floor(v * img.height);
	buffer = (int *)img.first_pixel;
	color = buffer[(y * img.size_line / (img.bits_per_pixel / 8)) + x];
	return (int_to_color(color));
}

// P is a point on the surface of the OBJ
// we apply the OBJ's color
// the texture
// the checkerboard
// the bumpmap?
t_vec3 compute_obj_material(t_object *obj, t_vec3 P)
{
	t_vec3 color;
	float uv[2];

	color = obj->color;

	if (obj->type == OBJ_SPHERE)
		sphere_uv(P, obj->data.sphere, &uv[0], &uv[1]);
	else if (obj->type == OBJ_PLANE)
		plane_uv(P, obj->data.plane, &uv[0], &uv[1]);
	else
		assert(0);

	// texture

	if (obj->texture.img_ptr)
		color = get_texture_pixel_at(obj->texture, uv[0], uv[1]);

	// luminosity

	// checkerboard
	if (obj->checker)
	{
		int u = floor(uv[0] * g_CHECKERBOARD_SCALE_FACTOR);
		int v = floor(uv[1] * g_CHECKERBOARD_SCALE_FACTOR);
		if ((u + v) % 2 != 0)
			color = (t_vec3){1 - color.x, 1 - color.y, 1 - color.z};
	}
	return (color);
}

// Change the memory value in the image
static void set_image_pixel_at(t_app *app, int x, int y, int color)
{
	int *buffer;
	int bytes_per_pixel;

	bytes_per_pixel = app->bits_per_pixel / 8;
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
			color = calc_pixel_color(app, x, y);
			set_image_pixel_at(app, x, y, color);
		}
	}
	mlx_put_image_to_window(app->mlx_ptr, app->win_ptr, app->img_ptr, 0, 0);
}

// Given a x,y pixel coordinate, calculate its color
int calc_pixel_color(t_app *app, int x, int y) // scene, not app
{
	t_vec3 d_vector;
	t_vec3 color;
	// t_object *obj;
	t_hit hit;

	d_vector = camera_pixel_to_vector(app->scene->global_cam, x, y);
	// obj = get_hit(app->scene, app->scene->global_cam.pos, d_vector, &dist);

	hit = intersect_objects(app->scene, app->scene->global_cam.pos, d_vector);
	
	color = (t_vec3){0, 0, 0};

	if (hit.obj) // we hit something
	{
		// float clamp;
		// clamp = fmaxf(0, 255.0f - dist);
		// color = (t_vec3){clamp / 255.0f, clamp / 255.0f, clamp / 255.0f}; // gray

		// calc UV and do diverse stuff
		// // object checkerboard pattern
		// color = compute_lighting(scene, hit, ray);
		t_vec3 P = t_to_P(app->scene->global_cam.pos, d_vector, hit.t);
		color = color_mult(color, compute_obj_material(hit.obj, P));
	}
	// if ambient light
	color = vec3_add(color, vec3_scale(app->scene->ambient_light.color, app->scene->ambient_light.brightness));
	return to_color_int(color);
}
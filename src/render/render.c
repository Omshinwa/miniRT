/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 11:09:42 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/16 09:49:38 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_private.h"

static const int	COLOR_TOGGLE = 1;
static const int	BUMP_TOGGLE = 16;

static t_vec3	int_to_color(int input)
{
	t_vec3	color;
	float	r;
	float	g;
	float	b;

	r = (input >> 16 & 0x0000FF);
	g = (input >> 8 & 0x0000FF);
	b = (input & 0x0000FF);
	color = (t_vec3){r / 255.0f, g / 255.0f, b / 255.0f};
	return (color);
}

static t_vec3	get_texture_pixel_at(t_img img, float u, float v)
{
	int	color;
	int	*buffer;
	int	x;
	int	y;

	x = ft_clamp((int)floor(u * img.width), 0, img.width - 1);
	y = ft_clamp((int)floor(v * img.height), 0, img.height - 1);
	buffer = (int *)img.first_pixel;
	color = buffer[(y * img.size_line / (img.bits_per_pixel / 8)) + x];
	return (int_to_color(color));
}

static t_vec3	compute_obj_material(t_app *app, t_object *obj, t_vec3 hp)
{
	t_vec3	color;
	float	uv[2];

	if (! (app->scene->render_mode & COLOR_TOGGLE))
		return ((t_vec3){0.5, 0.5, 0.5});
	color = obj->color;
	if (obj->type == OBJ_SPHERE)
		sphere_uv(hp, obj->data.sphere, &uv[0], &uv[1]);
	else if (obj->type == OBJ_PLANE)
		plane_uv(hp, obj->data.plane, &uv[0], &uv[1]);
	else if (obj->type == OBJ_CYLINDER)
		cylinder_uv(hp, obj->data.cylinder, &uv[0], &uv[1]);
	else if (obj->type == OBJ_CONE)
		cone_uv(hp, obj->data.cone, &uv[0], &uv[1]);
	else
		return (color);
	if (obj->texture.mlx_img)
		color = get_texture_pixel_at(obj->texture, uv[0], uv[1]);
	if (obj->checker)
	{
		if (((int)(uv[0] * CHECKERBOARD_SCALE_FACTOR)
			+ (int)(uv[1] * CHECKERBOARD_SCALE_FACTOR)) % 2 != 0)
			color = (t_vec3){1 - color.x, 1 - color.y, 1 - color.z};
	}
	return (color);
}

static t_vec3	get_normal(t_object *obj, t_vec3 *point, t_ray *ray)
{
	if (obj->type == OBJ_SPHERE)
		return (get_sphere_normal(&obj->data.sphere, point, ray));
	if (obj->type == OBJ_PLANE)
		return (get_plane_normal(&obj->data.plane, ray));
	if (obj->type == OBJ_CYLINDER)
		return (get_cylinder_normal(&obj->data.cylinder, point, ray));
	if (obj->type == OBJ_CONE)
		return (get_cone_normal(&obj->data.cone, point, ray));
	return (vec3_new(0, 1, 0));
}

int	calc_pixel_color(t_app *app, int x, int y)
{
	t_hit	hit;
	t_ray	ray;

	ray = create_cam_ray(app, &app->scene->camera, x, y);
	hit = get_hit(app->scene, ray);
	if (!hit.obj)
		return (0);
	hit.point = vec3_add(ray.origin, vec3_sca(ray.dir, hit.t));
	hit.normal = get_normal(hit.obj, &hit.point, &ray);
	hit.color = hit.obj->color;
	if (app->scene->render_mode & BUMP_TOGGLE)
		apply_bump_map(&hit);
	hit.color = compute_obj_material(app, hit.obj, hit.point);
	hit.color = compute_lighting(app->scene, &hit, &ray);
	if (hit.obj == app->target_object)
		hit.color = vec3_add(hit.color, (t_vec3){0.1, 0.1, 0});
	return (to_color_int(hit.color));
}

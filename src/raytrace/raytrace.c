/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytrace.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:18:22 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/10 10:24:22 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracing.h"

t_ray	create_cam_ray(const t_app *app, const t_camera *camera, int x, int y)
{
	t_ray		ray;
	t_vec3		p;
	float		aspect_ratio;
	float		fov_scale;

	aspect_ratio = (float)app->img.width / (float)app->img.height;
	fov_scale = tanf((camera->fov * M_PI / 180.0f) / 2.0f);
	p.x = (2.0f * ((float)x + 0.5f) / (float)app->img.width - 1.0f)
		* aspect_ratio * fov_scale;
	p.y = (1.0f - 2.0f * ((float)y + 0.5f) / (float)app->img.height)
		* fov_scale;
	p.z = 1;
	ray.dir = (t_vec3){
		p.x * camera->right.x + p.y * camera->up.x + p.z * camera->forward.x,
		p.x * camera->right.y + p.y * camera->up.y + p.z * camera->forward.y,
		p.x * camera->right.z + p.y * camera->up.z + p.z * camera->forward.z};
	ray.origin = camera->pos;
	ray.dir = vec3_normalize(ray.dir);
	return (ray);
}

float	get_hit_object(const t_ray ray, const t_object *obj)
{
	float	hit;

	hit = -1.0f;
	if (obj->type == OBJ_SPHERE)
		hit = get_hit_sphere(ray, obj->data.sphere);
	else if (obj->type == OBJ_CYLINDER)
		hit = get_hit_cylinder(ray, obj->data.cylinder);
	else if (obj->type == OBJ_PLANE)
		hit = get_hit_plane(ray, obj->data.plane);
	else if (obj->type == OBJ_CONE)
		hit = get_hit_cone(ray, obj->data.cone);
	return (hit);
}

t_hit	get_hit(const t_scene *scene, t_ray ray)
{
	t_hit	hit;
	float	t;
	int		i;

	hit.obj = NULL;
	hit.t = MAX_DRAW_DISTANCE;
	i = -1;
	while (++i < scene->obj_count)
	{
		t = get_hit_object(ray, &scene->objects[i]);
		if (t > 0 && t < hit.t)
		{
			hit.t = t;
			hit.obj = &scene->objects[i];
		}
	}
	return (hit);
}

t_object	*get_object_from_xy(t_app *app, int x, int y)
{
	t_ray	ray;
	t_hit	hit;

	ray = create_cam_ray(app, &app->scene->camera, x, y);
	hit = get_hit(app->scene, ray);
	return (hit.obj);
}

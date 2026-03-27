/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracing.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:18:31 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/18 06:29:05 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYTRACING_H
# define RAYTRACING_H

# include "scene_object.h"
# include "../app/app.h"

# define MAX_DRAW_DISTANCE 1000000
# define CHECKERBOARD_SCALE_FACTOR 10

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	dir;
}	t_ray;

typedef struct s_hit
{
	float		t;
	t_vec3		point;
	t_vec3		normal;
	t_vec3		color;
	t_object	*obj;
}	t_hit;

// object specific
float		get_hit_sphere(const t_ray ray, const t_sphere sphere);
void		sphere_uv(const t_vec3 P, const t_sphere obj,
				float *u, float *v);
t_vec3		get_sphere_normal(const t_sphere *sp, const t_vec3 *point,
				const t_ray *ray);

float		get_hit_plane(const t_ray ray, const t_plane pl);
void		plane_uv(const t_vec3 P, const t_plane pl,
				float *u, float *v);
t_vec3		get_plane_normal(const t_plane *pl, const t_ray *ray);

float		get_hit_cylinder(t_ray ray, t_cylinder cylinder);
void		cylinder_uv(const t_vec3 P, const t_cylinder cy,
				float *u, float *v);
t_vec3		get_cylinder_normal(const t_cylinder *cy, const t_vec3 *point,
				const t_ray *ray);

float		get_hit_cone(t_ray ray, t_cone cone);
void		cone_uv(t_vec3 P, t_cone cn, float *u, float *v);
t_vec3		get_cone_normal(const t_cone *cn, const t_vec3 *point,
				const t_ray *ray);

// raytrace utilities
float		get_hit_object(const t_ray ray, const t_object *obj);
t_ray		create_cam_ray(const t_app *app, const t_camera *camera,
				int x, int y);
t_hit		get_hit(const t_scene *scene, t_ray ray);

t_object	*get_object_from_xy(t_app *app, int x, int y);

#endif

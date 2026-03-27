/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_plane.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:18:05 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/13 07:07:00 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracing.h"

float	get_hit_plane(const t_ray ray, const t_plane pl)
{
	float	t;
	t_vec3	pl_to_c;
	float	denom;

	denom = vec3_dot(ray.dir, pl.normal);
	if (fabs(denom) < EPSILON)
		return (-1.0f);
	pl_to_c = vec3_sub(pl.pos, ray.origin);
	t = vec3_dot(pl_to_c, pl.normal) / denom;
	return (t);
}

void	plane_uv(const t_vec3 P, const t_plane pl, float *u, float *v)
{
	t_vec3	tmp;
	t_vec3	y_axis;
	t_vec3	x_axis;
	t_vec3	p_xyz_from_pl_pov;

	tmp = (t_vec3){1, 0, 0};
	if (fabs(vec3_dot(tmp, pl.normal)) > 1.0 - EPSILON)
		tmp = (t_vec3){0, 1, 0};
	y_axis = vec3_normalize(vec3_cross(pl.normal, tmp));
	x_axis = vec3_cross(y_axis, pl.normal);
	p_xyz_from_pl_pov = vec3_sub(P, pl.pos);
	*u = vec3_dot(p_xyz_from_pl_pov, x_axis);
	*v = vec3_dot(p_xyz_from_pl_pov, y_axis);
	*u /= CHECKERBOARD_SCALE_FACTOR * CHECKERBOARD_SCALE_FACTOR;
	*v /= CHECKERBOARD_SCALE_FACTOR * CHECKERBOARD_SCALE_FACTOR;
	*u -= floor(*u);
	*v = 1 - (*v - floor(*v));
}

t_vec3	get_plane_normal(const t_plane *pl, const t_ray *ray)
{
	if (vec3_dot(pl->normal, ray->dir) > 0)
		return (vec3_negate(pl->normal));
	return (pl->normal);
}

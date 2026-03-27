/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:18:12 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/10 07:59:32 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracing.h"

float	get_hit_sphere(const t_ray ray, const t_sphere sphere)
{
	t_quad	q;
	t_vec3	oc;

	oc = vec3_sub(ray.origin, sphere.pos);
	q.a = vec3_dot(ray.dir, ray.dir);
	q.b = 2.0f * vec3_dot(ray.dir, oc);
	q.c = vec3_dot(oc, oc) - sphere.radius * sphere.radius;
	quadratic_roots(&q);
	return (min_positive_f(q.t1, q.t2));
}

void	sphere_uv(const t_vec3 P, const t_sphere obj, float *u, float *v)
{
	t_vec3	local;

	local = vec3_normalize(vec3_sub(P, obj.pos));
	*u = 0.5 + atan2(local.z, local.x) / (2 * M_PI);
	*v = 0.5 - asin(local.y) / M_PI;
}

t_vec3	get_sphere_normal(const t_sphere *sp, const t_vec3 *point,
		const t_ray *ray)
{
	t_vec3	normal;

	normal = vec3_normalize(vec3_sub(*point, sp->pos));
	if (vec3_dot(normal, ray->dir) > 0)
		return (vec3_negate(normal));
	return (normal);
}

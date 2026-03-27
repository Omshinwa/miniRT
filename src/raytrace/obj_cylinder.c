/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 09:34:17 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/10 07:59:12 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracing.h"

static float	get_hit_disk(t_ray ray, t_disk di)
{
	float	t;
	t_vec3	p;
	t_vec3	p_disk_pov;

	t = get_hit_plane(ray, (t_plane){di.pos, di.normal});
	if (t < EPSILON)
		return (-1.0f);
	p = vec3_add(ray.origin, vec3_sca(ray.dir, t));
	p_disk_pov = vec3_sub(p, di.pos);
	if (vec3_dot(p_disk_pov, p_disk_pov) <= di.radius * di.radius)
		return (t);
	return (-1.0f);
}

static float	check_cylinder_height(t_ray ray, t_cylinder cyl, float t)
{
	t_vec3	p;
	float	y;

	if (t < 0.0f)
		return (-1.0f);
	p = vec3_add(ray.origin, vec3_sca(ray.dir, t));
	y = vec3_dot(vec3_sub(p, cyl.pos), cyl.axis);
	if (y < -cyl.height / 2 || y > cyl.height / 2)
		return (-1.0f);
	return (t);
}

static float	get_cylinder_caps(t_ray ray, t_cylinder cyl)
{
	t_vec3	cb;
	t_vec3	ct;
	float	t1;
	float	t2;

	cb = vec3_sub(cyl.pos, vec3_sca(cyl.axis, cyl.height / 2));
	ct = vec3_add(cyl.pos, vec3_sca(cyl.axis, cyl.height / 2));
	t1 = get_hit_disk(ray, (t_disk){cb, cyl.axis, cyl.radius});
	t2 = get_hit_disk(ray, (t_disk){ct, cyl.axis, cyl.radius});
	return (min_positive_f(t1, t2));
}

static t_quad	get_cylinder_quad(t_ray ray, t_cylinder cyl)
{
	t_vec3	d_perp;
	t_vec3	oc;
	t_vec3	oc_perp;
	t_quad	quad;

	d_perp = vec3_sub(ray.dir,
			vec3_sca(cyl.axis, vec3_dot(ray.dir, cyl.axis)));
	oc = vec3_sub(ray.origin, cyl.pos);
	oc_perp = vec3_sub(oc,
			vec3_sca(cyl.axis, vec3_dot(oc, cyl.axis)));
	quad.a = vec3_dot(d_perp, d_perp);
	quad.b = 2 * vec3_dot(d_perp, oc_perp);
	quad.c = vec3_dot(oc_perp, oc_perp) - cyl.radius * cyl.radius;
	return (quad);
}

float	get_hit_cylinder(t_ray ray, t_cylinder cyl)
{
	t_quad	quad;
	float	t_body;
	float	t_caps;

	quad = get_cylinder_quad(ray, cyl);
	quadratic_roots(&quad);
	quad.t1 = check_cylinder_height(ray, cyl, quad.t1);
	quad.t2 = check_cylinder_height(ray, cyl, quad.t2);
	t_body = min_positive_f(quad.t1, quad.t2);
	t_body = check_cylinder_height(ray, cyl, t_body);
	t_caps = get_cylinder_caps(ray, cyl);
	return (min_positive_f(t_body, t_caps));
}

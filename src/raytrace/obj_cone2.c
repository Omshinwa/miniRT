/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_cone2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 10:47:33 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/18 06:28:38 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracing.h"

// Return t if hit point is within cone height range, -1 otherwise.
static float	check_cone_height(t_ray ray, t_cone cone, float t)
{
	t_vec3	p;
	float	proj;

	if (t < 0.0f)
		return (-1.0f);
	p = vec3_add(ray.origin, vec3_sca(ray.dir, t));
	proj = vec3_dot(vec3_sub(p, cone.pos), cone.axis);
	if (proj < 0.0f || proj > cone.height)
		return (-1.0f);
	return (t);
}

// Ray-cone base intersection: hit the base plane, then check if inside radius.
static float	get_cone_base(t_ray ray, t_cone cone)
{
	t_vec3	base_center;
	float	base_radius;
	t_plane	base_plane;
	float	t;
	t_vec3	p;

	base_center = vec3_add(cone.pos, vec3_sca(cone.axis, cone.height));
	base_radius = tanf(cone.angle) * cone.height;
	base_plane = (t_plane){base_center, cone.axis};
	t = get_hit_plane(ray, base_plane);
	if (t < EPSILON)
		return (-1.0f);
	p = vec3_add(ray.origin, vec3_sca(ray.dir, t));
	if (vec3_len_sq(vec3_sub(p, base_center)) <= base_radius * base_radius)
		return (t);
	return (-1.0f);
}

// Compute quadratic coefficients for ray-cone intersection, relative to apex.
static t_quad	get_cone_quad(t_ray ray, t_cone cone)
{
	t_vec3	oc;
	float	da;
	float	oca;
	float	k;
	t_quad	q;

	k = cosf(cone.angle);
	k = k * k;
	oc = vec3_sub(ray.origin, cone.pos);
	da = vec3_dot(ray.dir, cone.axis);
	oca = vec3_dot(oc, cone.axis);
	q.a = da * da - k * vec3_dot(ray.dir, ray.dir);
	q.b = 2.0f * (da * oca - k * vec3_dot(ray.dir, oc));
	q.c = oca * oca - k * vec3_dot(oc, oc);
	return (q);
}

// Ray-cone intersection (body + base cap). Returns closest positive t.
float	get_hit_cone(t_ray ray, t_cone cone)
{
	t_quad	quad;
	float	t_body;
	float	t_base;

	quad = get_cone_quad(ray, cone);
	quadratic_roots(&quad);
	t_body = min_positive_f(
			check_cone_height(ray, cone, quad.t1),
			check_cone_height(ray, cone, quad.t2));
	t_base = get_cone_base(ray, cone);
	return (min_positive_f(t_body, t_base));
}

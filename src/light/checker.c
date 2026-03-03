/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 06:33:55 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/03 11:31:20 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static t_vec3 checker_sphere(t_hit *hit)
{
	t_sphere *sp;
	t_vec3 local;
	float u;
	float v;

	sp = &hit->obj->data.sphere;
	local = vec3_normalize(vec3_sub(hit->point, sp->center));
	u = 0.5 + atan2(local.z, local.x) / (2 * M_PI);
	v = 0.5 - asin(local.y) / M_PI;
	u = u * hit->obj->checker.scale;
	v = v * hit->obj->checker.scale;
	if (((int)floor(u) + (int)floor(v)) % 2 == 0)
		return (sp->color);
	return (hit->obj->checker.color2);
}

static t_vec3 checker_plane(t_hit *hit)
{
	float scale;
	int pattern;
	t_plane *pl;

	pl = &hit->obj->data.plane;
	scale = hit->obj->checker.scale;
	pattern = (int)floor(hit->point.x / scale) + (int)floor(hit->point.y / scale) + (int)floor(hit->point.z / scale);
	if (pattern % 2 == 0)
		return (pl->color);
	return (hit->obj->checker.color2);
}

t_vec3 get_checker_color(t_hit *hit)
{
	if (!hit->obj->checker.enabled)
		return (hit->color);
	if (hit->obj->type == OBJ_SPHERE)
		return (checker_sphere(hit));
	if (hit->obj->type == OBJ_PLANE)
		return (checker_plane(hit));
	return (hit->color);
}

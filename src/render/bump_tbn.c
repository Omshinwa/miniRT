/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_tbn.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 08:55:06 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/13 08:08:58 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_private.h"

static void	build_tbn_sphere(t_vec3 *t, t_vec3 *b, t_vec3 n, t_vec3 local)
{
	t_vec3	tmp;

	tmp = (t_vec3){-local.z, 0.0f, local.x};
	if (vec3_len_sq(tmp) < EPSILON)
		tmp = (t_vec3){1, 0, 0};
	*t = vec3_normalize(tmp);
	*b = vec3_cross(n, *t);
}

static void	build_tbn_plane(t_vec3 *t, t_vec3 *b, t_vec3 n)
{
	t_vec3	tmp;

	tmp = (t_vec3){1, 0, 0};
	if (fabsf(vec3_dot(tmp, n)) > 1.0f - EPSILON)
		tmp = (t_vec3){0, 1, 0};
	*b = vec3_normalize(vec3_cross(n, tmp));
	*t = vec3_cross(*b, n);
	*b = vec3_negate(*b);
}

static void	get_uv_and_tbn(t_hit *hit, float *uv, t_vec3 *tb)
{
	if (hit->obj->type == OBJ_SPHERE)
	{
		sphere_uv(hit->point, hit->obj->data.sphere, &uv[0], &uv[1]);
		build_tbn_sphere(&tb[0], &tb[1], hit->normal,
			vec3_normalize(vec3_sub(hit->point, hit->obj->data.sphere.pos)));
	}
	else if (hit->obj->type == OBJ_PLANE)
	{
		plane_uv(hit->point, hit->obj->data.plane, &uv[0], &uv[1]);
		build_tbn_plane(&tb[0], &tb[1], hit->normal);
	}
	else if (hit->obj->type == OBJ_CYLINDER)
	{
		cylinder_uv(hit->point, hit->obj->data.cylinder, &uv[0], &uv[1]);
		tb[0] = vec3_normalize(vec3_cross(hit->obj->data.cylinder.axis,
					hit->normal));
		tb[1] = vec3_cross(hit->normal, tb[0]);
	}
	else if (hit->obj->type == OBJ_CONE)
	{
		cone_uv(hit->point, hit->obj->data.cone, &uv[0], &uv[1]);
		tb[0] = vec3_normalize(vec3_cross(hit->obj->data.cone.axis,
					hit->normal));
		tb[1] = vec3_cross(hit->normal, tb[0]);
	}
}

void	apply_bump_map(t_hit *hit)
{
	float	uv[2];
	float	grad[2];
	t_vec3	tb[2];
	t_vec3	perturb;

	if (!hit->obj->bump_map.mlx_img)
		return ;
	get_uv_and_tbn(hit, uv, tb);
	compute_gradient(&hit->obj->bump_map, uv[0], uv[1], grad);
	perturb = vec3_add(
			vec3_sca(tb[0], -BUMP_STRENGTH * grad[0]),
			vec3_sca(tb[1], -BUMP_STRENGTH * grad[1]));
	hit->normal = vec3_normalize(vec3_add(hit->normal, perturb));
}

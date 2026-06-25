/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_cone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 09:25:48 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/18 06:26:40 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracing.h"

// Compute cone normal: base uses axis direction, body uses surface gradient.
// Flipped to always face the ray.
t_vec3	get_cone_normal(const t_cone *cn, const t_vec3 *point,
		const t_ray *ray)
{
	t_vec3	p_local;
	float	proj;
	float	k_sq;
	t_vec3	normal;

	p_local = vec3_sub(*point, cn->pos);
	proj = vec3_dot(p_local, cn->axis);
	if (fabsf(proj - cn->height) < EPSILON * 10)
	{
		normal = cn->axis;
		if (vec3_dot(normal, ray->dir) < 0)
			return (normal);
		return (vec3_negate(normal));
	}
	k_sq = cosf(cn->angle) * cosf(cn->angle);
	normal = vec3_normalize(vec3_sub(p_local,
				vec3_sca(cn->axis, proj / k_sq)));
	if (vec3_dot(normal, ray->dir) > 0)
		return (vec3_negate(normal));
	return (normal);
}

// u: horizontal position around the cone (0..1), computed from angle
// v: vertical position along the cone (0 at tip, 1 at base)
void	cone_uv(t_vec3 hp, t_cone cn, float *u, float *v)
{
	t_vec3	p_local;
	t_vec3	tmp;
	t_vec3	u_axis;
	t_vec3	v_axis;

	p_local = vec3_sub(hp, cn.pos);
	tmp = (t_vec3){0, 1, 0};
	if (fabsf(vec3_dot(tmp, cn.axis)) > 1.0f - EPSILON)
		tmp = (t_vec3){1, 0, 0};
	u_axis = vec3_normalize(vec3_cross(cn.axis, tmp));
	v_axis = vec3_cross(cn.axis, u_axis);
	*u = 0.5f + atan2f(vec3_dot(p_local, v_axis),
			vec3_dot(p_local, u_axis)) / (2.0f * M_PI);
	*v = ft_fclamp(vec3_dot(p_local, cn.axis) / cn.height, 0.0f, 1.0f);
}

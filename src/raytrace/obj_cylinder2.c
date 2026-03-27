/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_cylinder2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 09:49:48 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/10 07:59:49 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raytracing.h"

t_vec3	get_cylinder_normal(const t_cylinder *cy, const t_vec3 *point,
		const t_ray *ray)
{
	t_vec3	cp;
	float	proj;
	float	half_h;
	t_vec3	normal;

	cp = vec3_sub(*point, cy->pos);
	proj = vec3_dot(cp, cy->axis);
	half_h = cy->height * 0.5f;
	if (fabsf(proj - half_h) < EPSILON * 10.0f)
		normal = cy->axis;
	else if (fabsf(proj + half_h) < EPSILON * 10.0f)
		normal = vec3_negate(cy->axis);
	else
		normal = vec3_normalize(vec3_sub(cp, vec3_sca(cy->axis, proj)));
	if (vec3_dot(normal, ray->dir) > 0)
		return (vec3_negate(normal));
	return (normal);
}

void	cylinder_uv(const t_vec3 hp, const t_cylinder cy, float *u, float *v)
{
	t_vec3	cp;
	t_vec3	tmp;
	t_vec3	u_axis;
	t_vec3	v_axis;

	cp = vec3_sub(hp, cy.pos);
	tmp = (t_vec3){0, 1, 0};
	if (fabs(vec3_dot(tmp, cy.axis)) > 1.0f - EPSILON)
		tmp = (t_vec3){1, 0, 0};
	u_axis = vec3_normalize(vec3_cross(cy.axis, tmp));
	v_axis = vec3_cross(cy.axis, u_axis);
	*u = 0.5f + atan2f(vec3_dot(cp, v_axis),
			vec3_dot(cp, u_axis)) / (2.0f * M_PI);
	*v = (vec3_dot(cp, cy.axis) / cy.height) + 0.5f;
	*v = ft_fclamp(*v, 0.0f, 1.0f);
}

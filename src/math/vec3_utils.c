/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:27:24 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/09 12:27:25 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math.h"

t_vec3	vec3_negate(t_vec3 v)
{
	return (vec3_new(-v.x, -v.y, -v.z));
}

t_vec3	vec3_reflect(t_vec3 v, t_vec3 n)
{
	float	dot;

	dot = vec3_dot(v, n);
	return (vec3_sub(v, vec3_sca(n, 2.0 * dot)));
}

t_vec3	vec3_mul(t_vec3 a, t_vec3 b)
{
	t_vec3	result;

	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	return (result);
}

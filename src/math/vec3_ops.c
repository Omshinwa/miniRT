/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:16:49 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/09 12:16:50 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math.h"

float	vec3_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3	vec3_cross(t_vec3 a, t_vec3 b)
{
	t_vec3	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

float	vec3_len(t_vec3 v)
{
	return (sqrt(vec3_dot(v, v)));
}

float	vec3_len_sq(t_vec3 v)
{
	return (vec3_dot(v, v));
}

t_vec3	vec3_normalize(t_vec3 v)
{
	float	len;

	len = vec3_len(v);
	if (len < FLT_EPSILON)
		return (vec3_new(0, 0, 0));
	return (vec3_div(v, len));
}

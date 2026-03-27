/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_basic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:16:40 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/09 12:16:41 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math.h"

t_vec3	vec3_new(float x, float y, float z)
{
	t_vec3	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

t_vec3	vec3_add(t_vec3 a, t_vec3 b)
{
	return (vec3_new(a.x + b.x, a.y + b.y, a.z + b.z));
}

t_vec3	vec3_sub(t_vec3 a, t_vec3 b)
{
	return (vec3_new(a.x - b.x, a.y - b.y, a.z - b.z));
}

t_vec3	vec3_sca(t_vec3 v, float t)
{
	return (vec3_new(v.x * t, v.y * t, v.z * t));
}

t_vec3	vec3_div(t_vec3 v, float t)
{
	return (vec3_new(v.x / t, v.y / t, v.z / t));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_ops.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 06:33:47 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/03 11:31:20 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_vec3 color_add(t_vec3 a, t_vec3 b)
{
	t_vec3 result;

	result.r = a.r + b.r;
	result.g = a.g + b.g;
	result.b = a.b + b.b;
	return (result);
}

t_vec3 color_mul_scalar(t_vec3 c, float s)
{
	t_vec3 result;

	result.r = (int)(c.r * s);
	result.g = (int)(c.g * s);
	result.b = (int)(c.b * s);
	return (result);
}

t_vec3 color_mul(t_vec3 a, t_vec3 b)
{
	t_vec3 result;

	result.r = (int)(a.r * b.r / 255.0);
	result.g = (int)(a.g * b.g / 255.0);
	result.b = (int)(a.b * b.b / 255.0);
	return (result);
}

t_vec3 color_clamp(t_vec3 c)
{
	t_vec3 result;

	result.r = ft_clamp(c.r, 0, 255);
	result.g = ft_clamp(c.g, 0, 255);
	result.b = ft_clamp(c.b, 0, 255);
	return (result);
}

int color_to_int(t_vec3 c)
{
	t_vec3 clamped;

	clamped = color_clamp(c);
	return ((clamped.r << 16) | (clamped.g << 8) | clamped.b);
}

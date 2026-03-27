/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:16:13 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/16 10:05:16 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math.h"

float	min_positive_f(float t1, float t2)
{
	if (t1 > 0 && t2 > 0)
		return (fminf(t1, t2));
	if (t1 > 0)
		return (t1);
	if (t2 > 0)
		return (t2);
	return (-1.0f);
}

int	ft_clamp(int val, int min, int max)
{
	if (val < min)
		return (min);
	if (val > max)
		return (max);
	return (val);
}

float	ft_fclamp(float val, float min, float max)
{
	if (val < min)
		return (min);
	if (val > max)
		return (max);
	return (val);
}

bool	quadratic_roots(t_quad *q)
{
	float	d;
	float	sqrt_d;

	d = q->b * q->b - 4.0f * q->a * q->c;
	if (d < 0.0f)
	{
		q->t1 = -1.0f;
		q->t2 = -1.0f;
		return (false);
	}
	sqrt_d = sqrtf(d);
	q->t1 = (-q->b - sqrt_d) / (2.0f * q->a);
	q->t2 = (-q->b + sqrt_d) / (2.0f * q->a);
	return (true);
}

int	to_color_int(t_vec3 c)
{
	int	red;
	int	green;
	int	blue;
	int	color;

	red = ft_clamp((int)(255 * c.x), 0, 255);
	green = ft_clamp((int)(255 * c.y), 0, 255);
	blue = ft_clamp((int)(255 * c.z), 0, 255);
	color = (red << 16) | (green << 8) | blue;
	return (color);
}

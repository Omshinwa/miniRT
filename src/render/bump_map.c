/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 07:47:11 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/18 06:29:31 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_private.h"

// Sample grayscale height [0,1] from bump map at UV coordinates
static float	sample_height(t_img *img, float u, float v)
{
	int		x;
	int		y;
	int		color;
	int		*buffer;
	float	height;

	u = u - floorf(u);
	v = v - floorf(v);
	x = (int)(u * (img->width - 1));
	y = (int)(v * (img->height - 1));
	x = ft_clamp(x, 0, img->width -1);
	y = ft_clamp(y, 0, img->height - 1);
	buffer = (int *)(img->first_pixel);
	color = buffer[y * (img->size_line / 4) + x];
	height = (color >> 16 & 0xFF) + (color >> 8 & 0xFF) + (color & 0xFF);
	height /= (3.0f * 255.0f);
	return (height);
}

// Compute bump map gradient (dU, dV) via central differences
void	compute_gradient(t_img *img, float u, float v, float *grad)
{
	float	step_u;
	float	step_v;

	step_u = 1.0f / (float)img->width;
	step_v = 1.0f / (float)img->height;
	grad[0] = sample_height(img, u + step_u, v)
		- sample_height(img, u - step_u, v);
	grad[1] = sample_height(img, u, v + step_v)
		- sample_height(img, u, v - step_v);
}

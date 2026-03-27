/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:19:13 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/18 06:32:00 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
# define COMMON_H

# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

# include "../libft/libft.h"
# include "../minilibx-linux/mlx.h"

// 3D vector: used for positions, directions and colors (RGB)
typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
}	t_vec3;

// MLX image: pixel buffer and metadata for direct pixel access
typedef struct s_img
{
	void	*mlx_img;
	char	*first_pixel;
	int		size_line;
	int		bits_per_pixel;
	int		endian;
	int		width;
	int		height;

}	t_img;

#endif

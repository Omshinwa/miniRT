/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:16:25 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/12 08:43:36 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_H
# define MATH_H

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# include "../common.h"
# include <math.h>
# include <float.h>

# define EPSILON 1e-4

typedef struct s_quad
{
	float	a;
	float	b;
	float	c;
	float	t1;
	float	t2;
}	t_quad;

float	min_positive_f(float a, float b);
bool	quadratic_roots(t_quad *q);

int		ft_clamp(int val, int min, int max);
float	ft_fclamp(float val, float min, float max);

int		to_color_int(t_vec3 c);

t_vec3	vec3_new(float x, float y, float z);
t_vec3	vec3_add(t_vec3 a, t_vec3 b);
t_vec3	vec3_sub(t_vec3 a, t_vec3 b);
t_vec3	vec3_sca(t_vec3 v, float t);
t_vec3	vec3_div(t_vec3 v, float t);

float	vec3_dot(t_vec3 a, t_vec3 b);
t_vec3	vec3_cross(t_vec3 a, t_vec3 b);
float	vec3_len(t_vec3 v);
float	vec3_len_sq(t_vec3 v);
t_vec3	vec3_normalize(t_vec3 v);

t_vec3	vec3_negate(t_vec3 v);
t_vec3	vec3_reflect(t_vec3 v, t_vec3 n);
t_vec3	vec3_mul(t_vec3 a, t_vec3 b);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 06:33:39 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/03 12:30:36 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_H
#define LIGHT_H

#include "../math/math.h"
#include "../scene/scene.h"
#include "../render/render.h"

# define WIN_W 1200
# define WIN_H 800
# define EPSILON 1e-6
# define MAX_DIST 1e30
# define SPEC_POW 50
# define BG_COLOR 0x000000

typedef struct s_light_ctx
{
	t_vec3	light_dir;
	float	light_dist;
	float	diff;
	float	spec;
	t_vec3	result;
}	t_light_ctx;

int is_in_shadow(t_scene *scene, t_vec3 *point, t_vec3 *light_dir,
				 float light_dist);
t_vec3 apply_ambient(t_light *ambient, t_vec3 *obj_color);
t_vec3 apply_diffuse(t_light *light, t_hit *hit, t_light_ctx *ctx);
t_vec3 apply_specular(t_light *light, t_hit *hit,
					  t_ray *ray, t_light_ctx *ctx);
t_vec3 compute_lighting(t_scene *scene, t_hit *hit, t_ray *ray);
t_vec3 color_add(t_vec3 a, t_vec3 b);
t_vec3 color_mul_scalar(t_vec3 c, float s);
t_vec3 color_mul(t_vec3 a, t_vec3 b);
t_vec3 color_clamp(t_vec3 c);
int color_to_int(t_vec3 c);
t_vec3 get_checker_color(t_hit *hit);

#endif

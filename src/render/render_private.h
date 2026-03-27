/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_private.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 06:33:21 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/18 06:31:18 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_PRIVATE_H
# define RENDER_PRIVATE_H

# include "../raytrace/scene_object.h"
# include "../raytrace/raytracing.h"

// Phong specular exponent: higher = tighter highlight
# define SPEC_POW 50

// Bump map intensity: higher = more pronounced relief
# define BUMP_STRENGTH 3.0f

# define LIGHT_STRENGTH 1000

// Ambient light is just a light with no position
typedef t_light	t_ambient;

// Temp values recomputed for each light during shading
typedef struct s_light_ctx
{
	t_vec3	light_dir;
	float	light_dist;
	float	diff;
	float	spec;
	t_color	result;
}	t_light_ctx;

// Phong shading: ambient, shadow test, lighting accumulation
int		is_in_shadow(t_scene *scene, t_vec3 *point, t_vec3 *light_dir,
			float light_dist);
t_color	compute_lighting(t_scene *scene, t_hit *hit, t_ray *ray);

// Bump mapping: gradient sampling and normal perturbation
void	compute_gradient(t_img *img, float u, float v, float *grad);
void	apply_bump_map(t_hit *hit);

/************************************************
*	 				ENTRY POINT				 	*
*												*/
int		calc_pixel_color(t_app *app, int x, int y);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 06:33:31 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/18 06:30:27 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_private.h"

static const int	LIGHT_TOGGLE = 2;
static const int	SHADOW_TOGGLE = 4;
static const int	SPEC_TOGGLE = 8;

// Compute ambient component: object color * ambient color * brightness
static t_color	apply_ambient(t_scene *scene,
	t_light *ambient, t_color *obj_color)
{
	t_color	amb;

	if (!(scene->render_mode & LIGHT_TOGGLE))
		return ((t_vec3){0, 0, 0});
	amb = vec3_sca(*obj_color, ambient->brightness);
	amb = vec3_mul(amb, ambient->color);
	return (amb);
}

// Compute diffuse component (Lambertian) for one light
static t_color	apply_diffuse(t_scene *scene, t_light *light, t_hit *hit,
	t_light_ctx *ctx)
{
	t_color	diffuse;

	if (!(scene->render_mode & LIGHT_TOGGLE))
		return (hit->color);
	ctx->diff = ft_fclamp(vec3_dot(hit->normal, ctx->light_dir), 0.0, 1.0);
	diffuse = vec3_sca(hit->color, ctx->diff * light->brightness);
	diffuse = vec3_mul(diffuse, light->color);
	return (diffuse);
}

// Compute specular highlight (Phong) for one light
static t_color	apply_specular(t_light *light, t_hit *hit,
		t_ray *ray, t_light_ctx *ctx)
{
	t_vec3	reflect;
	t_vec3	view_dir;
	float	spec_dot;
	t_color	spec_color;

	reflect = vec3_reflect(vec3_negate(ctx->light_dir), hit->normal);
	view_dir = vec3_negate(ray->dir);
	spec_dot = ft_fclamp(vec3_dot(reflect, view_dir), 0.0, 1.0);
	ctx->spec = pow(spec_dot, SPEC_POW) * light->brightness;
	spec_color = vec3_sca(light->color, ctx->spec);
	return (spec_color);
}

// Sum diffuse + specular for one light, return black if in shadow
static t_color	compute_light_contrib(t_scene *scene, t_hit *hit,
		t_ray *ray, t_light *light)
{
	t_light_ctx	ctx;
	t_color		diffuse;
	t_color		specular;
	t_vec3		to_light;

	to_light = vec3_sub(light->pos, hit->point);
	ctx.light_dist = vec3_len(to_light);
	ctx.light_dir = vec3_normalize(to_light);
	if (scene->render_mode & SHADOW_TOGGLE
		&& is_in_shadow(scene, &hit->point, &ctx.light_dir, ctx.light_dist))
		return ((t_color){0, 0, 0});
	diffuse = apply_diffuse(scene, light, hit, &ctx);
	if (scene->render_mode & SPEC_TOGGLE)
		specular = apply_specular(light, hit, ray, &ctx);
	else
		specular = (t_vec3){0, 0, 0};
	ctx.result = vec3_add(diffuse, specular);
	if (scene->render_mode & LIGHT_TOGGLE)
		ctx.result = vec3_sca(ctx.result, LIGHT_STRENGTH
				/ ctx.light_dist / ctx.light_dist);
	return (ctx.result);
}

// Accumulate ambient + all lights contributions (Phong shading)
t_color	compute_lighting(t_scene *scene, t_hit *hit, t_ray *ray)
{
	t_color	result;
	t_color	contrib;
	t_light	*lights;
	int		i;

	result = apply_ambient(scene, &scene->ambient_light, &hit->color);
	lights = scene->lights;
	i = -1;
	while (++i < scene->light_count)
	{
		contrib = compute_light_contrib(scene, hit, ray, &lights[i]);
		if (scene->render_mode & LIGHT_TOGGLE)
			result = vec3_add(result, contrib);
		else
			result = vec3_add(result,
					vec3_sca(contrib, 1.0f / scene->light_count));
	}
	return (result);
}

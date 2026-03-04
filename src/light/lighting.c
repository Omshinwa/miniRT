/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 06:33:31 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/04 07:17:14 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "light.h"

t_vec3	vec3_negate(t_vec3 v)
{
	return (vec3_new(-v.x, -v.y, -v.z));
}

t_vec3	vec3_reflect(t_vec3 v, t_vec3 n)
{
	double	dot;

	dot = vec3_dot(v, n);
	return (vec3_sub(v, vec3_mul(n, 2.0 * dot)));
}


t_vec3 apply_ambient(t_light *ambient, t_vec3 *obj_color)
{
	t_vec3 amb;

	amb = color_mul_scalar(*obj_color, ambient->brightness);
	amb = color_mul(amb, ambient->color);
	return (amb);
}

t_vec3 apply_diffuse(t_light *light, t_hit *hit, t_light_ctx *ctx)
{
	t_vec3 diffuse;

	ctx->diff = ft_dclamp(vec3_dot(hit->normal, ctx->light_dir), 0.0, 1.0);
	diffuse = color_mul_scalar(hit->color, ctx->diff * light->brightness);
	diffuse = color_mul(diffuse, light->color);
	return (diffuse);
}

t_vec3 apply_specular(t_light *light, t_hit *hit,
					  t_ray *ray, t_light_ctx *ctx)
{
	t_vec3 reflect;
	t_vec3 view_dir;
	float spec_dot;
	t_vec3 spec_color;

	reflect = vec3_reflect(vec3_negate(ctx->light_dir), hit->normal);
	view_dir = vec3_normalize(vec3_sub(ray->origin, hit->point));
	spec_dot = ft_dclamp(vec3_dot(reflect, view_dir), 0.0, 1.0);
	ctx->spec = pow(spec_dot, SPEC_POW) * light->brightness;
	spec_color = color_mul_scalar(light->color, ctx->spec);
	return (spec_color);
}

static t_vec3 compute_light_contrib(t_scene *scene, t_hit *hit,
									t_ray *ray, t_light *light)
{
	t_light_ctx ctx;
	t_vec3 diffuse;
	t_vec3 specular;
	t_vec3 to_light;

	to_light = vec3_sub(light->pos, hit->point);
	ctx.light_dist = vec3_len(to_light);
	ctx.light_dir = vec3_normalize(to_light);
	if (is_in_shadow(scene, &hit->point, &ctx.light_dir, ctx.light_dist))
		return ((t_vec3){0, 0, 0});
	diffuse = apply_diffuse(light, hit, &ctx);
	specular = apply_specular(light, hit, ray, &ctx);
	return (color_add(diffuse, specular));
}

t_vec3 compute_lighting(t_scene *scene, t_hit *hit, t_ray *ray)
{
	t_vec3 result;
	t_vec3 contrib;
	t_light *light;

	result = apply_ambient(&scene->ambient_light, &hit->color);
	light = scene->lights;
	int i = -1;
	while (++i < scene->number_of_light)
	{
		contrib = compute_light_contrib(scene, hit, ray, light);
		result = color_add(result, contrib);
	}
	return (result);
}
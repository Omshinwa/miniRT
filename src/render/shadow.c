/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 06:33:22 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/18 06:31:43 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_private.h"

// Check if point is occluded from light by any scene object
int	is_in_shadow(t_scene *scene, t_vec3 *point, t_vec3 *light_dir,
		float light_dist)
{
	t_ray		shadow_ray;
	t_object	*objs;
	float		t;
	int			i;

	shadow_ray.origin = vec3_add(*point, vec3_sca(*light_dir, EPSILON));
	shadow_ray.dir = *light_dir;
	objs = scene->objects;
	i = -1;
	while (++i < scene->obj_count)
	{
		t = get_hit_object(shadow_ray, &objs[i]);
		if (t > EPSILON && t < light_dist)
			return (1);
	}
	return (0);
}

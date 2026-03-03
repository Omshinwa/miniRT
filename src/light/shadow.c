/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 06:33:22 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/03 11:30:17 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static int try_shadow_hit(t_ray *ray, t_object *obj, float *t)
{
	if (obj->type == OBJ_SPHERE)
		return (intersect_sphere(ray, &obj->data.sphere, t));
	if (obj->type == OBJ_PLANE)
		return (intersect_plane(ray, &obj->data.plane, t));
	return (0);
}

int is_in_shadow(t_scene *scene, t_vec3 *point, t_vec3 *light_dir,
				 float light_dist)
{
	t_ray shadow_ray;
	t_object *obj;
	float t;

	shadow_ray.origin = vec3_add(*point, vec3_mul(*light_dir, EPSILON));
	shadow_ray.dir = *light_dir;
	obj = scene->objects;
	while (obj)
	{
		t = 0;
		if (try_shadow_hit(&shadow_ray, obj, &t))
			if (t > EPSILON && t < light_dist)
				return (1);
		obj = obj->next;
	}
	return (0);
}

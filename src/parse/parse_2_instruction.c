/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_2_instruction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:17:29 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/12 13:04:18 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_private.h"

static const t_field	g_obj_fields[4] = {
{"color or texture", T_RGB_OR_TEXTURE, true, offsetof(t_object, color)},
{"checkerboard", T_IS_CHECKERBOARD, false, offsetof(t_object, checker)},
{"bump map", T_TEXTURE, false, offsetof(t_object, bump_map)},
{NULL, T_INVALID, false, 0},
};

static bool	push_object(t_scene *scene, const t_object obj)
{
	t_object	*tmp;

	tmp = safe_calloc(scene->obj_count + 1, sizeof(t_object));
	if (!tmp)
		return (false);
	ft_memcpy(tmp, scene->objects, scene->obj_count * sizeof(t_object));
	free(scene->objects);
	scene->objects = tmp;
	scene->objects[scene->obj_count] = obj;
	scene->obj_count++;
	return (true);
}

static bool	add_light(t_scene *scene, void **dest)
{
	t_light	*tmp;

	tmp = safe_calloc(scene->light_count + 1, sizeof(t_light));
	if (!tmp)
		return (false);
	ft_memcpy(&tmp[1], scene->lights, scene->light_count * sizeof(t_light));
	free(scene->lights);
	scene->lights = tmp;
	scene->light_count++;
	*dest = (void *)scene->lights;
	return (true);
}

static bool	camera_derive_basis(t_camera *cam)
{
	t_vec3	world_up;

	if (vec3_len(cam->forward) < 1 - EPSILON
		|| (vec3_len(cam->forward) > 1 + EPSILON))
		return (print_err("Camera orientation vector isn't normalized.\n",
				0, false));
	world_up = (t_vec3){0, 1, 0};
	if (fabsf(vec3_dot(cam->forward, world_up)) > 1.0f - EPSILON)
		world_up = (t_vec3){0, 0, 1};
	cam->right = vec3_normalize(vec3_cross(world_up, cam->forward));
	cam->up = vec3_normalize(vec3_cross(cam->forward, cam->right));
	return (true);
}

bool	do_shape_instruction(t_app *app, t_parse_args *args)
{
	t_object		obj;

	obj = (t_object){};
	obj.type = args->type;
	args->dest = &obj.data;
	if (!parse_fields(app, args) || !args->tokens[args->i])
		return (false);
	args->fields = g_obj_fields;
	args->dest = &obj;
	if (!parse_fields(app, args) || !push_object(app->scene, obj))
		return (false);
	if (args->tokens[args->i] != NULL)
	{
		print_err("too many arguments : @ \n",
			args->tokens[args->i], false);
		return (false);
	}
	return (true);
}

bool	do_non_shape_instruction(t_app *app, t_parse_args *args)
{
	if (args->type == OBJ_LIGHT
		&& !add_light(app->scene, &args->dest))
		return (false);
	if (!parse_fields(app, args))
		return (false);
	if (args->type == OBJ_CAMERA
		&& !camera_derive_basis(&app->scene->camera))
		return (false);
	return (true);
}

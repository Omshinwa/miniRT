/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_object.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 06:41:12 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/11 11:32:57 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_OBJECT_H
# define SCENE_OBJECT_H

# include "../math/math.h"
# include <stdbool.h>

typedef t_vec3	t_color;

typedef struct s_camera
{
	t_vec3	pos;
	t_vec3	right;
	t_vec3	up;
	t_vec3	forward;
	int		fov;
}	t_camera;

typedef struct s_sphere
{
	t_vec3	pos;
	float	radius;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	pos;
	t_vec3	normal;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	pos;
	t_vec3	axis;
	float	radius;
	float	height;
}	t_cylinder;

typedef struct s_cone
{
	t_vec3	pos;
	t_vec3	axis;
	float	angle;
	float	height;
}	t_cone;

typedef struct s_disk
{
	t_vec3	pos;
	t_vec3	normal;
	float	radius;
}	t_disk;

typedef union u_any_obj
{
	t_sphere	sphere;
	t_plane		plane;
	t_cylinder	cylinder;
	t_cone		cone;
}	t_any_obj;

typedef enum e_obj_type
{
	OBJ_SPHERE = 1,
	OBJ_PLANE = 2,
	OBJ_CYLINDER = 4,
	OBJ_CONE = 8,
	OBJ_IS_SHAPE = 15,
	OBJ_LIGHT = 16,
	OBJ_AMBIANT = 32,
	OBJ_CAMERA = 64,
}	t_obj_type;

typedef struct s_object
{
	t_obj_type	type;
	t_any_obj	data;
	t_vec3		color;
	t_img		texture;
	bool		checker;
	t_img		bump_map;
}	t_object;

typedef struct s_light
{
	t_vec3	pos;
	float	brightness;
	t_vec3	color;
}	t_light;

typedef struct s_scene
{
	t_camera	camera;
	t_object	*objects;
	int			obj_count;
	t_light		*lights;
	int			light_count;
	t_light		ambient_light;
	int			render_mode;
}	t_scene;

#endif

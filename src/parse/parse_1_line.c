/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_1_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:17:21 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/10 10:54:24 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_private.h"

enum e_parse_constants
{
	INSTRUCTION_COUNT = 7,
};

static const t_instruction	g_camera_instruction = {
	"C", OBJ_CAMERA, true,
	offsetof(t_scene, camera),
{{"pos", T_VEC, true, offsetof(t_camera, pos)},
{"dir", T_UNIT, true, offsetof(t_camera, forward)},
{"fov", T_FOV, true, offsetof(t_camera, fov)},
{NULL, T_INVALID, false, 0}},
};

static const t_instruction	g_sphere_instruction = {
	"sp", OBJ_SPHERE, false,
	offsetof(t_object, data),
{{"pos", T_VEC, true, offsetof(t_sphere, pos)},
{"diameter", T_DIAMETER, true, offsetof(t_sphere, radius)},
{NULL, T_INVALID, false, 0}},
};

static const t_instruction	g_plane_instruction = {
	"pl", OBJ_PLANE, false,
	offsetof(t_object, data),
{
{"point", T_VEC, true, offsetof(t_plane, pos)},
{"normal", T_UNIT, true, offsetof(t_plane, normal)},
{NULL, T_INVALID, false, 0},
}};

static const t_instruction	g_cylinder_instruction = {
	"cy", OBJ_CYLINDER, false,
	offsetof(t_object, data),
{
{"pos", T_VEC, true, offsetof(t_cylinder, pos)},
{"axis", T_UNIT, true, offsetof(t_cylinder, axis)},
{"diameter", T_DIAMETER, true, offsetof(t_cylinder, radius)},
{"height", T_FLOAT, true, offsetof(t_cylinder, height)},
{NULL, T_INVALID, false, 0},
}};

static const t_instruction	g_cone_instruction = {
	"cn", OBJ_CONE, false,
	offsetof(t_object, data),
{
{"pos", T_VEC, true, offsetof(t_cone, pos)},
{"axis", T_UNIT, true, offsetof(t_cone, axis)},
{"angle", T_FLOAT, true, offsetof(t_cone, angle)},
{"height", T_FLOAT, true, offsetof(t_cone, height)},
{NULL, T_INVALID, false, 0},
}};

static const t_instruction	g_light_instruction = {
	"L", OBJ_LIGHT, false,
	offsetof(t_scene, lights),
{{"pos", T_VEC, true, offsetof(t_light, pos)},
{"brightness", T_FLOAT_UNIT, true, offsetof(t_light, brightness)},
{"color", T_RGB, true, offsetof(t_light, color)},
{NULL, T_INVALID, false, 0}},
};

static const t_instruction	g_ambient_instruction = {
	"A", OBJ_AMBIANT, true,
	offsetof(t_scene, ambient_light),
{{"brightness", T_FLOAT_UNIT, true, offsetof(t_light, brightness)},
{"color", T_RGB, true, offsetof(t_light, color)},
{NULL, T_INVALID, false, 0}},
};

static const t_instruction	*g_all_instructions[INSTRUCTION_COUNT] = {
	&g_camera_instruction,
	&g_ambient_instruction,
	&g_light_instruction,
	&g_sphere_instruction,
	&g_plane_instruction,
	&g_cylinder_instruction,
	&g_cone_instruction,
};

static bool	do_instruction(char **tokens, t_app *app
	, t_instruction instruction)
{
	t_parse_args	args;

	args.type = instruction.type;
	args.tokens = tokens;
	args.fields = instruction.fields;
	args.i = 1;
	args.dest = (char *)app->scene + instruction.scene_target;
	if (instruction.type & OBJ_IS_SHAPE)
		return (do_shape_instruction(app, &args));
	else
		return (do_non_shape_instruction(app, &args));
}

static void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

// verify uniqueness of 'A' 'C' etc...
static bool	check_uniqueness(char *token, const t_instruction *instruction)
{
	static bool	unique[INSTRUCTION_COUNT];
	int			i;

	if (!instruction->should_be_unique)
		return (true);
	i = -1;
	while (++i < INSTRUCTION_COUNT)
	{
		if (instruction->type == g_all_instructions[i]->type)
		{
			if (unique[i])
				return (print_err("error, duplicated field: @\n", token, false));
			unique[i] = true;
			break ;
		}
	}
	return (true);
}

bool	parse_line(char *line, t_app *app)
{
	char	**tokens;
	int		i;

	while (*line == ' ' || *line == '\t')
		line++;
	if (*line == '\0' || *line == '#')
		return (true);
	tokens = ft_split(line, ' ');
	if (!tokens || !tokens[0])
		return (free_tokens(tokens), false);
	i = -1;
	while (++i < INSTRUCTION_COUNT)
	{
		if (ft_strncmp(tokens[0], g_all_instructions[i]->id, -1) == 0)
		{
			if (check_uniqueness(tokens[0], g_all_instructions[i])
				&& do_instruction(tokens, app, *g_all_instructions[i]))
				return (free_tokens(tokens), true);
			else
				return (free_tokens(tokens), false);
		}
	}
	print_err("unknown identifier @ \n", tokens[0], false);
	return (free_tokens(tokens), false);
}

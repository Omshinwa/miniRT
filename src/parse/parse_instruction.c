#include "../main.h"
#include "parse_private.h"

/* Defining global constant static that will be instructions */
static const t_instruction g_camera_instruction = {
	"C",
	offsetof(t_scene, global_cam),
	{{"pos", T_VEC, true, offsetof(t_camera, pos)},
	 {"dir", T_UNIT, true, offsetof(t_camera, forward)},
	 {"fov", T_FOV, true, offsetof(t_camera, fov)},
	 {NULL, T_INVALID, false, 0}},
};

static const t_field g_object_field[] = {
	{"color", T_RGB, true, offsetof(t_object, color)},
	{NULL, T_INVALID, false, 0},
};

static const t_instruction g_sphere_instruction = {
	"sp",
	offsetof(t_scene, objects),
	{{"pos", T_VEC, true, offsetof(t_sphere, pos)},
	 {"diameter", T_DIAMETER, true, offsetof(t_sphere, r)},
	 {NULL, T_INVALID, false, 0}},
};

static const t_instruction g_plane_instruction = {
	"pl",
	offsetof(t_scene, objects),
	{
		{"point", T_VEC, true, offsetof(t_plane, pos)},
		{"normal", T_UNIT, true, offsetof(t_plane, normal)},
		{NULL, T_INVALID, false, 0},
	}};

static const t_instruction g_cylinder_instruction = {
	"cy",
	offsetof(t_scene, objects),
	{
		{"pos", T_VEC, true, offsetof(t_cylinder, pos)},
		{"axis", T_UNIT, true, offsetof(t_cylinder, axis)},
		{"diameter", T_DIAMETER, true, offsetof(t_cylinder, radius)},
		{"height", T_FLOAT, true, offsetof(t_cylinder, height)},
		{NULL, T_INVALID, false, 0},
	}};

static const t_instruction g_light_instruction = {
	"L",
	offsetof(t_scene, light),
	{{"pos", T_VEC, true, offsetof(t_light, pos)},
	 {"brightness", T_FLOAT, true, offsetof(t_light, brightness)},
	 {"color", T_RGB, false, offsetof(t_light, color)},
	 {NULL, T_INVALID, false, 0}},
};

static const t_instruction g_ambient_instruction = {
	"A",
	offsetof(t_scene, ambient_light),
	{{"brightness", T_FLOAT, true, offsetof(t_light, brightness)},
	 {"color", T_RGB, false, offsetof(t_light, color)},
	 {NULL, T_INVALID, false, 0}},
};

static bool push_object(t_scene *scene, t_object obj)
{
	t_object *tmp;

	tmp = malloc((scene->number_of_obj + 1) * sizeof(t_object));
	if (!tmp)
		return (false);
	ft_memcpy(tmp, scene->objects, scene->number_of_obj * sizeof(t_object));
	free(scene->objects);
	scene->objects = tmp;
	scene->objects[scene->number_of_obj] = obj;
	scene->number_of_obj++;
	return (true);
}

static bool do_object_instruction(char **tokens, t_scene *scene, t_instruction instruction)
{
	int color_idx;
	t_object obj;

	obj = (t_object){OBJ_SPHERE, {0}, {0}};
	color_idx = -1;

	if (instruction.id == g_sphere_instruction.id)
	{
		obj = (t_object){OBJ_SPHERE, {0}, {0}};
		color_idx = parse_fields(1, tokens, instruction.fields, &obj.data.sphere);
	}

	if (color_idx < 0 || !tokens[color_idx])
		return (false);

	color_idx = parse_fields(color_idx, tokens, g_object_field, &obj);

	if (color_idx < 0 || !push_object(scene, obj))
		return (false);
	return (true);
}

// Because we are only given the forward vector, we derive the up and right from
// it.
static int camera_derive_basis(t_camera *cam)
{
	t_vec3 world_up;

	if (vec3_length(cam->forward) < 0.99 || (vec3_length(cam->forward) > 1.01))
	{
		printf("Cameria orientation vector isn't normalized.\n");
		return (1);
	}
	world_up = (t_vec3){0, 1, 0};
	/* if forward is almost parallel to world_up, use a different reference */
	if (fabsf(dot_product(cam->forward, world_up)) > 0.99f)
		world_up = (t_vec3){0, 0, 1};
	cam->right = vec3_normalize(vec3_cross(world_up, cam->forward));
	cam->up = vec3_normalize(vec3_cross(cam->forward, cam->right));
	return (0);
}

static bool do_non_object_instruction(char **tokens, t_scene *scene, t_instruction instruction)
{
	int res;

	res = parse_fields(1, tokens, instruction.fields, (char *)scene + instruction.scene_target);
	if (res < 0)
		return (false);
	// if it was a camera, do an additional instruction
	if (instruction.id == g_camera_instruction.id)
	{
		if (camera_derive_basis(&scene->global_cam))
			return (false);
	}
	return (true);
}

/* ── helpers ─────────────────────────────────────────────────────────────── */

// could be added to the libft directly
static void free_tokens(char **tokens)
{
	int i;

	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

bool parse_line(char *line, t_scene *scene)
{
	char **tokens;

	while (*line == ' ' || *line == '\t')
		line++;
	if (*line == '\0' || *line == '#') // ignores comments
		return (true);

	/* split on spaces; tokens[0] = identifier, tokens[1..] = fields */
	tokens = ft_split(line, ' ');
	if (!tokens || !tokens[0])
		return (free_tokens(tokens), false);

	t_instruction lights[] = {g_camera_instruction, g_ambient_instruction, g_light_instruction};
	t_instruction objects[] = {g_sphere_instruction, g_plane_instruction, g_cylinder_instruction};
	size_t i;
	i = 0;
	// place lights and cam
	while (i < sizeof(lights) / sizeof(t_instruction))
	{
		if (ft_strncmp(tokens[0], lights[i].id, -1) == 0)
		{
			if (!do_non_object_instruction(tokens, scene, lights[i]))
				return (free_tokens(tokens), false);
			return (free_tokens(tokens), true);
		}
		i++;
	}
	i = 0;
	// add objects
	while (i < sizeof(objects) / sizeof(t_instruction))
	{
		if (ft_strncmp(tokens[0], objects[i].id, -1) == 0)
		{
			if (!do_object_instruction(tokens, scene, objects[i]))
				return (free_tokens(tokens), false);
			return (free_tokens(tokens), true);
		}
		i++;
	}
	ft_printf("unknown identifier '%s'\n", tokens[0]);
	return (free_tokens(tokens), false);
}
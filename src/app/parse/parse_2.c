/* ========================================================================== */
/* parse_2.c  –  table-driven scene file parser                               */
/*                                                                             */
/* HOW IT WORKS                                                                */
/* Each line is split into space-separated tokens with ft_split.              */
/* A key table per identifier maps token index → (type, dst offset).         */
/* A read_float() cursor-reader replaces strtof/strtol/sscanf/strtok_r.      */
/* Only libft + ft_memcpy / ft_strncmp / ft_isdigit are used.                */
/* ========================================================================== */

#include "../main.h"
#include "app.h"
#include <fcntl.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h> /* perror */

/* ── token types ─────────────────────────────────────────────────────────── */

typedef enum e_token_type
{
	T_VEC,		/* "x,y,z"  → t_vec3  (any range)           */
	T_UNIT,		/* "x,y,z"  → t_vec3  (components ∈ [-1,1]) */
	T_FLOAT,	/* "f"      → float                          */
	T_DIAMETER, /* "d"      → float stored as radius d/2     */
	T_FOV,		/* "n"      → int [0, 180]                   */
	T_RGB,		/* "r,g,b"  → t_vec3 scaled 0-1              */
	T_INVALID,
} t_token_type;

/* ── field descriptor ────────────────────────────────────────────────────── */

typedef struct s_key
{
	const char *name;
	t_token_type type;
	bool required;
	size_t offset;
} t_key;

/* ── key tables ──────────────────────────────────────────────────────────── */

static const t_key g_keys_camera[] = {
	{"pos", T_VEC, true, offsetof(t_camera, pos)},
	{"dir", T_UNIT, true, offsetof(t_camera, forward)},
	{"fov", T_FOV, true, offsetof(t_camera, fov)},
	{NULL, T_INVALID, false, 0}};

static const t_key g_keys_light[] = {
	{"pos", T_VEC, true, offsetof(t_light, pos)},
	{"brightness", T_FLOAT, true, offsetof(t_light, brightness)},
	{"color", T_RGB, false, offsetof(t_light, color)},
	{NULL, T_INVALID, false, 0}};

static const t_key g_keys_sphere[] = {
	{"center", T_VEC, true, offsetof(t_sphere, pos)},
	{"diameter", T_DIAMETER, true, offsetof(t_sphere, r)},
	{NULL, T_INVALID, false, 0}};

static const t_key g_keys_plane[] = {
	{"point", T_VEC, true, offsetof(t_plane, pos)},
	{"normal", T_UNIT, true, offsetof(t_plane, normal)},
	{NULL, T_INVALID, false, 0}};

static const t_key g_keys_cylinder[] = {
	{"center", T_VEC, true, offsetof(t_cylinder, pos)},
	{"axis", T_UNIT, true, offsetof(t_cylinder, axis)},
	{"diameter", T_DIAMETER, true, offsetof(t_cylinder, radius)},
	{"height", T_FLOAT, true, offsetof(t_cylinder, height)},
	{NULL, T_INVALID, false, 0}};

/* ── low-level number readers ────────────────────────────────────────────── */
/*
 * read_float: parse one float from *s, advance *s past the digits.
 * Returns 1 on success, 0 on failure.
 */
static int read_float(const char **s, float *out)
{
	float result;
	float frac;
	int sign;

	result = 0.0f;
	sign = 1;
	if (**s == '-')
	{
		sign = -1;
		(*s)++;
	}
	else if (**s == '+')
		(*s)++;
	if (!ft_isdigit(**s) && **s != '.')
		return (0);
	while (ft_isdigit(**s))
		result = result * 10.0f + (float)(*(*s)++ - '0');
	if (**s == '.')
	{
		(*s)++;
		frac = 0.1f;
		while (ft_isdigit(**s))
		{
			result += (float)(*(*s)++ - '0') * frac;
			frac *= 0.1f;
		}
	}
	*out = (float)sign * result;
	return (1);
}

/* read "x,y,z" from s */
static int read_vec3(const char *s, t_vec3 *out)
{
	float x;
	float y;
	float z;

	if (!read_float(&s, &x) || *s != ',')
		return (0);
	s++;
	if (!read_float(&s, &y) || *s != ',')
		return (0);
	s++;
	if (!read_float(&s, &z))
		return (0);
	out->x = x;
	out->y = y;
	out->z = z;
	return (1);
}

/* read "r,g,b" ints 0-255, store as [0,1] vec3 */
static int read_rgb(const char *s, t_vec3 *out)
{
	float r;
	float g;
	float b;

	if (!read_float(&s, &r) || *s != ',')
		return (0);
	s++;
	if (!read_float(&s, &g) || *s != ',')
		return (0);
	s++;
	if (!read_float(&s, &b))
		return (0);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (0);
	out->x = r / 255.0f;
	out->y = g / 255.0f;
	out->z = b / 255.0f;
	return (1);
}

/* ── token converter ─────────────────────────────────────────────────────── */

static bool convert_token(const char *tok, t_token_type type, void *field)
{
	t_vec3 v;
	float f;
	int i;

	if (type == T_VEC || type == T_UNIT)
	{
		if (!read_vec3(tok, &v))
			return (false);
		ft_memcpy(field, &v, sizeof(t_vec3));
	}
	else if (type == T_FLOAT || type == T_DIAMETER)
	{
		if (!read_float(&tok, &f))
			return (false);
		if (type == T_DIAMETER)
			f /= 2.0f;
		ft_memcpy(field, &f, sizeof(float));
	}
	else if (type == T_FOV)
	{
		if (!read_float(&tok, &f))
			return (false);
		i = (int)f;
		if (i < 0 || i > 180)
			return (false);
		ft_memcpy(field, &i, sizeof(int));
	}
	else if (type == T_RGB)
	{
		if (!read_rgb(tok, &v))
			return (false);
		ft_memcpy(field, &v, sizeof(t_vec3));
	}
	else
		return (false);
	return (true);
}

/* ── generic field parser ────────────────────────────────────────────────── */
/*
 * tokens: NULL-terminated array from ft_split(line, ' ').
 * Walks the key table, converting tokens[i] for each field.
 * Returns the index of the first unconsumed token, or -1 on error.
 */
static int parse_fields(char **tokens, const t_key *keys, void *dst)
{
	const t_key *k;
	int i;

	k = keys;
	i = 0;
	while (k->name)
	{
		if (!tokens[i])
		{
			if (k->required)
			{
				ft_printf("parse error: missing required field '%s'\n", k->name);
				return (-1);
			}
			k++;
			continue;
		}
		if (!convert_token(tokens[i], k->type, (char *)dst + k->offset))
		{
			ft_printf("parse error: bad value '%s' for '%s'\n",
					  tokens[i], k->name);
			return (-1);
		}
		i++;
		k++;
	}
	return (i);
}

/* ── helpers ─────────────────────────────────────────────────────────────── */

static void free_tokens(char **tokens)
{
	int i;

	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

static void camera_derive_basis(t_camera *cam)
{
	t_vec3 world_up;

	world_up = (t_vec3){0, 1, 0};
	/* if forward is almost parallel to world_up, use a different reference */
	if (fabsf(dot_product(cam->forward, world_up)) > 0.99f)
		world_up = (t_vec3){0, 0, 1};
	cam->right = vec3_normalize(vec3_cross(cam->forward, world_up));
	cam->up = vec3_normalize(vec3_cross(cam->right, cam->forward));
}

/* ── object helpers ──────────────────────────────────────────────────────── */

static bool push_object(t_scene *scene, t_object obj)
{
	t_object *tmp;

	tmp = realloc(scene->objects,
				  (scene->number_of_obj + 2) * sizeof(t_object));
	if (!tmp)
		return (false);
	scene->objects = tmp;
	scene->objects[scene->number_of_obj++] = obj;
	return (true);
}

/* ── line dispatcher ─────────────────────────────────────────────────────── */

static bool parse_line(char *line, t_scene *scene)
{
	char **tokens;
	t_object obj;
	t_light light;
	int color_idx;

	while (*line == ' ' || *line == '\t')
		line++;
	if (*line == '\0' || *line == '#')
		return (true);

	/* split on spaces; tokens[0] = identifier, tokens[1..] = fields */
	tokens = ft_split(line, ' ');
	if (!tokens || !tokens[0])
		return (free_tokens(tokens), false);

	if (ft_strncmp(tokens[0], "C", 2) == 0)
	{
		if (parse_fields(tokens + 1, g_keys_camera, &scene->global_cam) < 0)
			return (free_tokens(tokens), false);
		camera_derive_basis(&scene->global_cam);
	}
	else if (ft_strncmp(tokens[0], "L", 2) == 0)
	{
		light = (t_light){0};
		if (parse_fields(tokens + 1, g_keys_light, &light) < 0)
			return (free_tokens(tokens), false);
		(void)light; /* TODO: push to scene->lights */
	}
	else if (ft_strncmp(tokens[0], "sp", 3) == 0)
	{
		obj = (t_object){OBJ_SPHERE, {0}, {0}};
		color_idx = parse_fields(tokens + 1, g_keys_sphere, &obj.data.sphere);
		if (color_idx < 0 || !tokens[1 + color_idx] || !convert_token(tokens[1 + color_idx], T_RGB, &obj.color) || !push_object(scene, obj))
			return (free_tokens(tokens), false);
	}
	else if (ft_strncmp(tokens[0], "pl", 3) == 0)
	{
		obj = (t_object){OBJ_PLANE, {0}, {0}};
		color_idx = parse_fields(tokens + 1, g_keys_plane, &obj.data.plane);
		if (color_idx < 0 || !tokens[1 + color_idx] || !convert_token(tokens[1 + color_idx], T_RGB, &obj.color) || !push_object(scene, obj))
			return (free_tokens(tokens), false);
	}
	else if (ft_strncmp(tokens[0], "cy", 3) == 0)
	{
		obj = (t_object){OBJ_CYLINDER, {0}, {0}};
		color_idx = parse_fields(tokens + 1, g_keys_cylinder, &obj.data.cylinder);
		if (color_idx < 0 || !tokens[1 + color_idx] || !convert_token(tokens[1 + color_idx], T_RGB, &obj.color) || !push_object(scene, obj))
			return (free_tokens(tokens), false);
	}
	else
		ft_printf("parse warning: unknown identifier '%s'\n", tokens[0]);
	free_tokens(tokens);
	return (true);
}

/* ── public entry point ──────────────────────────────────────────────────── */

char *get_next_line_strip_nl(int fd);

int parse_scene_file(t_scene *scene, const char *filename)
{
	int fd;
	char *line;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	line = get_next_line_strip_nl(fd);
	while (line)
	{
		if (!parse_line(line, scene))
		{
			ft_printf("failed to parse line: %s\n", line);
			free(line);
			close(fd);
			return (-1);
		}
		free(line);
		line = get_next_line_strip_nl(fd);
	}
	close(fd);
	return (0);
}

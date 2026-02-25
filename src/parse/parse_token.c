// A bunch of converter functions from string to t_vec3 etc...
// It does output parameter to OUT.
#include "../main.h"
#include "parse_private.h"
#include <stddef.h>

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

// Given a string (tok), the type to convert it (type)
// Copy it to *dest
static bool convert_token(const char *tok, t_token_type type, void *dest)
{
	t_vec3 v;
	float f;
	int i;

	if (type == T_VEC || type == T_UNIT)
	{
		if (!read_vec3(tok, &v))
			return (false);
		ft_memcpy(dest, &v, sizeof(t_vec3));
	}
	else if (type == T_FLOAT || type == T_DIAMETER)
	{
		if (!read_float(&tok, &f))
			return (false);
		if (type == T_DIAMETER)
			f /= 2.0f;
		ft_memcpy(dest, &f, sizeof(float));
	}
	else if (type == T_FOV)
	{
		if (!read_float(&tok, &f))
			return (false);
		i = (int)f;
		if (i < 0 || i > 180)
			return (false);
		ft_memcpy(dest, &i, sizeof(int));
	}
	else if (type == T_RGB)
	{
		if (!read_rgb(tok, &v))
			return (false);
		ft_memcpy(dest, &v, sizeof(t_vec3));
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
int parse_fields(int index, char **tokens, const t_field *fields, void *dest)
{
	int i;

	i = index;
	while (fields->name)
	{
		if (!tokens[i])
		{
			if (fields->required)
			{
				ft_printf("parse error: missing required field '%s'\n", fields->name);
				return (-1);
			}
			fields++;
			continue;
		}
		if (!convert_token(tokens[i], fields->type, (char *)dest + fields->offset))
		{
			ft_printf("parse error: bad value '%s' for '%s'\n",
					  tokens[i], fields->name);
			return (-1);
		}
		i++;
		fields++;
	}
	return (i);
}
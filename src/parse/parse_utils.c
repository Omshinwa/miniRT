// A bunch of converter functions from string to t_vec3 etc...
// It does output parameter to OUT.
#include "../main.h"
#include "parse.h"
/* ── low-level number readers ────────────────────────────────────────────── */
/*
 * read_float: parse one float from *s, advance *s past the digits.
 * Returns 1 on success, 0 on failure.
 */
int read_float(const char **s, float *out)
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
int read_vec3(const char *s, t_vec3 *out)
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
int read_rgb(const char *s, t_vec3 *out)
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
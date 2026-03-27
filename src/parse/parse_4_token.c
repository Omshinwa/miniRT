/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_4_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:17:49 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/10 07:04:55 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_private.h"

static float	read_float_digits(char **s)
{
	float	result;
	float	frac;

	result = 0.0f;
	while (ft_isdigit(**s))
		result = result * 10.0f + (float)(*(*s)++ - '0');
	if (**s != '.')
		return (result);
	(*s)++;
	frac = 0.1f;
	while (ft_isdigit(**s))
	{
		result += (**s - '0') * frac;
		frac *= 0.1f;
		(*s)++;
	}
	return (result);
}

bool	read_float(char **s, float *out)
{
	int		sign;

	sign = 1;
	if (**s == '\0')
		return (false);
	if (**s == '-')
	{
		sign = -1;
		(*s)++;
	}
	else if (**s == '+')
		(*s)++;
	*out = (float)sign * read_float_digits(s);
	return (true);
}

bool	read_vec3(char *s, t_vec3 *out)
{
	float	x;
	float	y;
	float	z;

	if (!read_float(&s, &x) || *s != ',')
		return (false);
	s++;
	if (!read_float(&s, &y) || *s != ',')
		return (false);
	s++;
	if (!read_float(&s, &z))
		return (false);
	out->x = x;
	out->y = y;
	out->z = z;
	if (*s)
		return (print_err("unexpected @ in vec3 \n", s, false));
	return (true);
}

bool	read_rgb(char *s, t_vec3 *out)
{
	float	r;
	float	g;
	float	b;

	if (!read_float(&s, &r) || *s != ',')
		return (false);
	s++;
	if (!read_float(&s, &g) || *s != ',')
		return (false);
	s++;
	if (!read_float(&s, &b))
		return (false);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (0);
	out->x = r / 255.0f;
	out->y = g / 255.0f;
	out->z = b / 255.0f;
	if (*s)
		return (false);
	return (true);
}

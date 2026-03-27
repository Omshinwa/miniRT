/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_3_field.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:17:41 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/10 07:04:21 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_private.h"
#include <stddef.h>

static bool	process_vector(char *tok, t_token_type type, void *dest)
{
	t_vec3	v;

	if (!read_vec3(tok, &v))
		return (false);
	if (type == T_UNIT && vec3_len(v) < FLT_EPSILON)
	{
		print_err("error: @ is not a normalized vector. \n", tok, false);
		return (false);
	}
	if (type == T_UNIT
		&& (vec3_len(v) > 1 + EPSILON || vec3_len(v) < 1 - EPSILON))
	{
		print_err("warning: @ is not a normalized vector. \n", tok, true);
		v = vec3_normalize(v);
	}
	ft_memcpy(dest, &v, sizeof(t_vec3));
	return (true);
}

static bool	process_float(char *tok, t_token_type type, void *dest)
{
	float	f;
	int		i;
	char	*tok_original_pointer;

	tok_original_pointer = tok;
	if (!read_float(&tok, &f) || *tok != '\0')
		return (false);
	if (type == T_FOV)
	{
		i = (int)f;
		if (i < 0 || i > 180)
			return (false);
		ft_memcpy(dest, &i, sizeof(int));
		return (true);
	}
	else if (type == T_DIAMETER)
		f /= 2.0f;
	ft_memcpy(dest, &f, sizeof(float));
	if (type == T_FLOAT_UNIT)
	{
		if (f < 0 || f > 1)
			return (print_err("warning: @ is not in [0,1]. \n",
					tok_original_pointer, true));
	}
	return (true);
}

static bool	process_rgb_texture(t_app *app, char *tok, t_token_type type,
	void *dest)
{
	t_vec3	v;
	t_img	texture;

	if (type == T_RGB)
	{
		if (!read_rgb(tok, &v))
			return (false);
		ft_memcpy(dest, &v, sizeof(t_vec3));
	}
	else if (type == T_TEXTURE)
	{
		texture.mlx_img = mlx_xpm_file_to_image(app->mlx, tok,
				&texture.width, &texture.height);
		if (!texture.mlx_img)
			return (false);
		texture.first_pixel = mlx_get_data_addr(texture.mlx_img,
				&texture.bits_per_pixel, &texture.size_line, &texture.endian);
		ft_memcpy(dest, &texture, sizeof(t_img));
	}
	return (true);
}

static bool	process_token(t_app *app, char *tok, t_token_type type, void *dest)
{
	bool	b;

	if (type == T_VEC || type == T_UNIT)
		return (process_vector(tok, type, dest));
	else if (type == T_FLOAT || type == T_DIAMETER || type == T_FOV
		|| type == T_FLOAT_UNIT)
		return (process_float(tok, type, dest));
	else if (type == T_RGB || type == T_TEXTURE)
		return (process_rgb_texture(app, tok, type, dest));
	else if (type == T_IS_CHECKERBOARD)
	{
		b = true;
		if (ft_strncmp(tok, "check", -1))
			return (false);
		ft_memcpy(dest, &b, sizeof(bool));
	}
	else if (type == T_RGB_OR_TEXTURE)
	{
		if (process_token(app, tok, T_RGB, dest))
			return (true);
		if (process_token(app, tok, T_TEXTURE, dest + sizeof(t_vec3)))
			return (true);
		return (false);
	}
	return (true);
}

bool	parse_fields(t_app *app, t_parse_args *args)
{
	while (args->fields->name)
	{
		if (!args->tokens[args->i])
		{
			if (args->fields->required)
				return (print_err("parse error: missing required field @ \n",
						args->fields->name, false));
		}
		else if (!process_token(app, args->tokens[args->i], args->fields->type,
				(char *)args->dest + args->fields->offset))
		{
			if (args->fields->required)
				return (print_err("parse error: bad value @ for ",
						args->tokens[args->i], false),
					print_err("@ \n", args->fields->name, false));
		}
		else
			args->i++;
		args->fields++;
	}
	return (true);
}

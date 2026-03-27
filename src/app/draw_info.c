/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_info.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:15:21 by wiwu              #+#    #+#             */
/*   Updated: 2026/03/16 10:09:35 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"

static void	mlx_int_put(t_app *app, int x, int y, int data)
{
	char				*str;
	const unsigned int	color_red = 0xffff0000;

	str = ft_itoa(data);
	mlx_string_put(app->mlx, app->win, x, y, color_red, str);
	free(str);
}

static void	draw_vec3(t_app *app, int x, int y, t_vec3 data)
{
	mlx_int_put(app, x, y, data.x);
	mlx_int_put(app, x + 30, y, data.y);
	mlx_int_put(app, x + 60, y, data.z);
}

static void	draw_render(t_app *app)
{
	char				*str;
	int					i;
	const char			*render_passes[6] = {"COLOR", "LIGHT", "SHADOW",
		"SPEC", "BUMP", NULL};
	const unsigned int	color_red = 0xffff0000;

	i = 0;
	while (render_passes[i])
	{
		mlx_string_put(app->mlx, app->win, 120 + i * 70, 20, color_red, "(");
		mlx_int_put(app, 125 + i * 70, 20, i + 1);
		mlx_string_put(app->mlx, app->win, 130 + i * 70, 20, color_red, ")");
		str = ft_strdup(render_passes[i]);
		mlx_string_put(app->mlx, app->win, 140 + i * 70, 20, color_red, str);
		free(str);
		if (app->scene->render_mode & 1 << i)
			mlx_string_put(app->mlx, app->win, 140 + i * 70, 35,
				color_red, "[X]");
		else
			mlx_string_put(app->mlx, app->win, 140 + i * 70, 35,
				color_red, "[ ]");
		i++;
	}
}

void	draw_info(t_app *app)
{
	char				*obj_to_move;
	t_vec3				pos;
	const unsigned int	color_red = 0xffff0000;

	mlx_int_put(app, 20, 20, update_time(app));
	mlx_string_put(app->mlx, app->win, 40, 20, color_red, "FPS");
	obj_to_move = "CAMERA";
	if (app->target_object)
	{
		obj_to_move = type_to_str(app->target_object->type);
		pos = app->target_object->data.cone.pos;
		mlx_string_put(app->mlx, app->win, 20, 60, color_red, "axis");
		draw_vec3(app, 50, 60,
			vec3_sca(app->target_object->data.cylinder.axis, 100));
	}
	else
	{
		pos = app->scene->camera.pos;
		mlx_string_put(app->mlx, app->win, 80, 40, color_red, "fov");
		mlx_int_put(app, 110, 40, app->scene->camera.fov);
	}
	mlx_string_put(app->mlx, app->win, 20, 40, color_red, obj_to_move);
	mlx_string_put(app->mlx, app->win, 20, 50, color_red, "pos");
	draw_vec3(app, 50, 50, pos);
	draw_render(app);
}

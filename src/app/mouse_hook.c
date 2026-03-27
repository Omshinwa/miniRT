/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 08:09:24 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/12 15:17:32 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../app/app.h"

static const int	MOUSE_LEFT = 1;
static const int	MOUSE_RIGHT = 3;
static const int	MOUSE_WHEEL_UP = 4;
static const int	MOUSE_WHEEL_DN = 5;
static const int	MOV_STRENGTH = 2;

static t_vec3	*get_move_target(t_app *app)
{
	if (app->target_object)
		return (&app->target_object->data.cone.pos);
	return (&app->scene->camera.pos);
}

static void	apply_wheel_movement(t_app *app, t_vec3 *target, int keycode)
{
	t_vec3	delta;

	delta = vec3_sca(app->scene->camera.up, MOV_STRENGTH);
	if (keycode == MOUSE_WHEEL_UP)
		*target = vec3_add(*target, delta);
	else if (keycode == MOUSE_WHEEL_DN)
		*target = vec3_sub(*target, delta);
}

int	on_mouse_input(int keycode, int mouse_x, int mouse_y, t_app *app)
{
	t_vec3	*target;

	target = get_move_target(app);
	if (keycode == MOUSE_WHEEL_UP || keycode == MOUSE_WHEEL_DN)
		apply_wheel_movement(app, target, keycode);
	else if (keycode == MOUSE_LEFT)
		app->target_object = get_object_from_xy(app, mouse_x, mouse_y);
	else if (keycode == MOUSE_RIGHT)
		app->target_object = NULL;
	else
		printf("got %i key input \n", keycode);
	redraw(app);
	return (0);
}

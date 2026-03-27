/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 08:22:32 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/18 08:04:20 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../app/app.h"

static const int	KEY_ESC = 65307;
static const int	KEY_NUMPAD_0 = 65438;
static const int	KEY_NUMPAD_1 = 65436;
static const int	KEY_NUMPAD_2 = 65433;
static const int	KEY_NUMPAD_3 = 65435;
static const int	KEY_NUMPAD_4 = 65430;
static const int	KEY_NUMPAD_5 = 65437;
static const int	KEY_NUMPAD_6 = 65432;
static const int	KEY_NUMPAD_7 = 65429;
static const int	KEY_NUMPAD_8 = 65431;
static const int	KEY_NUMPAD_9 = 65434;
static const int	KEY_NUMPAD_PLUS = 65451;
static const int	KEY_NUMPAD_MINUS = 65453;
static const int	KEY_Q = 113;
static const int	KEY_W = 119;
static const int	KEY_E = 101;
static const int	KEY_S = 115;
static const int	KEY_A = 97;
static const int	KEY_D = 100;
static const int	KEY_ARROW_LEFT = 65361;
static const int	KEY_ARROW_UP = 65362;
static const int	KEY_ARROW_RIGHT = 65363;
static const int	KEY_ARROW_DOWN = 65364;

static const int	ALL_TOGGLE = 2147483647;
static const int	MOV_STRENGTH = 2;
static const float	ROT_STRENGTH = 0.1f;

static void	rotate_camera(t_camera *cam, int keycode)
{
	if (keycode == KEY_W)
		camera_pitch(cam, -ROT_STRENGTH);
	else if (keycode == KEY_S)
		camera_pitch(cam, ROT_STRENGTH);
	else if (keycode == KEY_A)
		camera_yaw(cam, -ROT_STRENGTH);
	else if (keycode == KEY_D)
		camera_yaw(cam, ROT_STRENGTH);
	else if (keycode == KEY_E)
		camera_roll(cam, -ROT_STRENGTH);
	else if (keycode == KEY_Q)
		camera_roll(cam, ROT_STRENGTH);
}

static void	handle_rotation(t_app *app, int keycode)
{
	t_vec3	*target;

	if (app->target_object->type & OBJ_PLANE
		|| app->target_object->type & OBJ_CYLINDER
		|| app->target_object->type & OBJ_CONE)
		target = &app->target_object->data.cylinder.axis;
	else
		return ;
	if (keycode == KEY_W)
		rotate_around_axis(target, (t_vec3){1, 0, 0}, ROT_STRENGTH);
	else if (keycode == KEY_S)
		rotate_around_axis(target, (t_vec3){1, 0, 0}, -ROT_STRENGTH);
	else if (keycode == KEY_A)
		rotate_around_axis(target, (t_vec3){0, 1, 0}, ROT_STRENGTH);
	else if (keycode == KEY_D)
		rotate_around_axis(target, (t_vec3){0, 1, 0}, -ROT_STRENGTH);
	else if (keycode == KEY_E)
		rotate_around_axis(target, (t_vec3){0, 0, 1}, ROT_STRENGTH);
	else if (keycode == KEY_Q)
		rotate_around_axis(target, (t_vec3){0, 0, 1}, -ROT_STRENGTH);
	else
		return ;
	*target = vec3_normalize(*target);
}

static void	handle_translation(t_app *app, t_camera *cam,
	int keycode)
{
	t_vec3	*target;

	if (app->target_object)
		target = &app->target_object->data.cone.pos;
	else
		target = &cam->pos;
	if (keycode == KEY_ARROW_UP)
		*target = vec3_add(*target, vec3_sca(cam->forward, MOV_STRENGTH));
	else if (keycode == KEY_ARROW_DOWN)
		*target = vec3_sub(*target, vec3_sca(cam->forward, MOV_STRENGTH));
	else if (keycode == KEY_ARROW_LEFT)
		*target = vec3_sub(*target, vec3_sca(cam->right, MOV_STRENGTH));
	else if (keycode == KEY_ARROW_RIGHT)
		*target = vec3_add(*target, vec3_sca(cam->right, MOV_STRENGTH));
}

static void	handle_render(t_app *app, int keycode)
{
	if (keycode == KEY_NUMPAD_1)
		app->scene->render_mode ^= 1;
	else if (keycode == KEY_NUMPAD_2)
		app->scene->render_mode ^= 2;
	else if (keycode == KEY_NUMPAD_3)
		app->scene->render_mode ^= 4;
	else if (keycode == KEY_NUMPAD_4)
		app->scene->render_mode ^= 8;
	else if (keycode == KEY_NUMPAD_5)
		app->scene->render_mode ^= 16;
	else if (keycode == KEY_NUMPAD_6)
		app->scene->render_mode ^= 32;
	else if (keycode == KEY_NUMPAD_7)
		app->scene->render_mode ^= 64;
	else if (keycode == KEY_NUMPAD_8)
		app->scene->render_mode ^= 128;
	else if (keycode == KEY_NUMPAD_9)
		app->scene->render_mode = ALL_TOGGLE;
	else if (keycode == KEY_NUMPAD_0)
		app->scene->render_mode = 0;
}

int	on_key_input(int keycode, t_app *app)
{
	t_camera	*cam;

	printf("key: %i \n", keycode);
	cam = &app->scene->camera;
	if (keycode == KEY_ESC)
		exit_n_clean(app, NULL);
	if (app->target_object)
		handle_rotation(app, keycode);
	else
		rotate_camera(cam, keycode);
	handle_translation(app, cam, keycode);
	handle_render(app, keycode);
	if (keycode == KEY_NUMPAD_PLUS)
		cam->fov += 10;
	else if (keycode == KEY_NUMPAD_MINUS)
		cam->fov -= 10;
	cam->fov = ft_clamp(cam->fov, 10, 170);
	redraw(app);
	return (0);
}

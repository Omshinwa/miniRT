#include "hooks.h"
#include "../draw/draw.h"
#include "../app/app.h"

//  █████   █████    ███████       ███████    █████   ████  █████████
// ▒▒███   ▒▒███   ███▒▒▒▒▒███   ███▒▒▒▒▒███ ▒▒███   ███▒  ███▒▒▒▒▒███
//  ▒███    ▒███  ███     ▒▒███ ███     ▒▒███ ▒███  ███   ▒███    ▒▒▒
//  ▒███████████ ▒███      ▒███▒███      ▒███ ▒███████    ▒▒█████████
//  ▒███▒▒▒▒▒███ ▒███      ▒███▒███      ▒███ ▒███▒▒███    ▒▒▒▒▒▒▒▒███
//  ▒███    ▒███ ▒▒███     ███ ▒▒███     ███  ▒███ ▒▒███   ███    ▒███
//  █████   █████ ▒▒▒███████▒   ▒▒▒███████▒   █████ ▒▒████▒▒█████████
// ▒▒▒▒▒   ▒▒▒▒▒    ▒▒▒▒▒▒▒       ▒▒▒▒▒▒▒    ▒▒▒▒▒   ▒▒▒▒  ▒▒▒▒▒▒▒▒▒

#include <stdio.h>

static void print_cam(t_app *app)
{
	printf("pos: %f %f %f ", app->global_cam.pos.x, app->global_cam.pos.y, app->global_cam.pos.z);
	printf("fov: %i \n", app->global_cam.fov);
}

int on_mouse_input(int keycode, int mouse_x, int mouse_y, t_app *app)
{
	(void)mouse_x;
	(void)mouse_y;
	(void)app;
	if (keycode == MOUSE_WHEEL_UP)
		app->global_cam.pos.z += C_KEY_STRENGTH;
	else if (keycode == MOUSE_WHEEL_DN)
		app->global_cam.pos.z -= C_KEY_STRENGTH;
	else if (keycode == MOUSE_LEFT)
		printf("sphere: %f %f %f \n", g_s.pos.x, g_s.pos.y, g_s.pos.z);
	else
		printf("got %i key input \n", keycode);
	redraw(app);
	print_cam(app);
	return (0);
}

int on_no_input(t_app *app)
{
	(void)app;
	return (0);
}

int on_key_input(int keycode, t_app *app)
{
	if (keycode == ESC_KEY)
		exit_n_clean(app);
	printf("got %i key input \n", keycode);
	if (keycode == KEY_W)
		app->global_cam.pos.z += C_KEY_STRENGTH;
	else if (keycode == KEY_S)
		app->global_cam.pos.z -= C_KEY_STRENGTH;
	else if (keycode == KEY_A)
		app->global_cam.pos.x -= C_KEY_STRENGTH * WINDOW_RATIO;
	else if (keycode == KEY_D)
		app->global_cam.pos.x += C_KEY_STRENGTH * WINDOW_RATIO;

	else if (keycode == KEY_ARROW_UP)
		app->global_cam.pos.y += C_KEY_STRENGTH;
	else if (keycode == KEY_ARROW_DOWN)
		app->global_cam.pos.y -= C_KEY_STRENGTH;
	else if (keycode == KEY_ARROW_LEFT)
		app->global_cam.pos.x -= C_KEY_STRENGTH * WINDOW_RATIO;
	else if (keycode == KEY_ARROW_RIGHT)
		app->global_cam.pos.x += C_KEY_STRENGTH * WINDOW_RATIO;

	// FOV
	else if (keycode == NUMPAD_PLUS)
		app->global_cam.fov += 10;
	else if (keycode == NUMPAD_MINUS)
		app->global_cam.fov -= 10;
	print_cam(app);
	redraw(app);
	return (0);
}
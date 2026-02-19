#include "../main.h"
#include "hooks.h"

// keycodes (X11)
static const int g_ESC_KEY = 65307;
static const int g_NUMPAD_PLUS = 65451;
static const int g_NUMPAD_MINUS = 65453;
// static const int g_KEY_R = 114;
static const int g_KEY_W = 119;
static const int g_KEY_S = 115;
static const int g_KEY_A = 97;
static const int g_KEY_D = 100;

static const int g_KEY_ARROW_LEFT = 65361;
static const int g_KEY_ARROW_UP = 65362;
static const int g_KEY_ARROW_RIGHT = 65363;
static const int g_KEY_ARROW_DOWN = 65364;

// numpad keys
// static const int g_NUMPAD_0 = 65438;
// static const int g_NUMPAD_1 = 65436;
// static const int g_NUMPAD_2 = 65433;
// static const int g_NUMPAD_3 = 65435;
// static const int g_NUMPAD_4 = 65430;
// static const int g_NUMPAD_5 = 65437;
// static const int g_NUMPAD_6 = 65432;
// static const int g_NUMPAD_7 = 65429;
// static const int g_NUMPAD_DIV = 65455;
// static const int g_NUMPAD_MUL = 65450;

// mouse buttons
static const int g_MOUSE_LEFT = 1;
// static const int g_MOUSE_MIDDLE = 2;
// static const int g_MOUSE_RIGHT = 3;
static const int g_MOUSE_WHEEL_UP = 4;
static const int g_MOUSE_WHEEL_DN = 5;

static const int g_KEY_STRENGTH = 10;

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
	if (keycode == g_MOUSE_WHEEL_UP)
		app->global_cam.pos.z += g_KEY_STRENGTH;
	else if (keycode == g_MOUSE_WHEEL_DN)
		app->global_cam.pos.z -= g_KEY_STRENGTH;
	else if (keycode == g_MOUSE_LEFT)
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
	if (keycode == g_ESC_KEY)
		exit_n_clean(app);
	printf("got %i key input \n", keycode);

	// Change cam orientation
	if (keycode == g_KEY_W)
		app->global_cam.pos.z += g_KEY_STRENGTH;
	else if (keycode == g_KEY_S)
		app->global_cam.pos.z -= g_KEY_STRENGTH;
	else if (keycode == g_KEY_A)
		app->global_cam.pos.x -= g_KEY_STRENGTH * WINDOW_RATIO;
	else if (keycode == g_KEY_D)
		app->global_cam.pos.x += g_KEY_STRENGTH * WINDOW_RATIO;

	// Change cam position
	else if (keycode == g_KEY_ARROW_UP)
		app->global_cam.pos.y += g_KEY_STRENGTH;
	else if (keycode == g_KEY_ARROW_DOWN)
		app->global_cam.pos.y -= g_KEY_STRENGTH;
	else if (keycode == g_KEY_ARROW_LEFT)
		app->global_cam.pos.x -= g_KEY_STRENGTH * WINDOW_RATIO;
	else if (keycode == g_KEY_ARROW_RIGHT)
		app->global_cam.pos.x += g_KEY_STRENGTH * WINDOW_RATIO;

	// FOV
	else if (keycode == g_NUMPAD_PLUS)
		app->global_cam.fov += 10;
	else if (keycode == g_NUMPAD_MINUS)
		app->global_cam.fov -= 10;
	print_cam(app);
	redraw(app);
	return (0);
}
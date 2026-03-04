#include "../main.h"
#include "../app/app.h"
#include "../scene/scene.h"

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
// static const int g_MOUSE_LEFT = 1;
// static const int g_MOUSE_MIDDLE = 2;
// static const int g_MOUSE_RIGHT = 3;
static const int g_MOUSE_WHEEL_UP = 4;
static const int g_MOUSE_WHEEL_DN = 5;

static const int g_MOV_STRENGTH = 10;
static const float g_ROT_STRENGTH = 0.05; // in radian

//  █████   █████    ███████       ███████    █████   ████  █████████
// ▒▒███   ▒▒███   ███▒▒▒▒▒███   ███▒▒▒▒▒███ ▒▒███   ███▒  ███▒▒▒▒▒███
//  ▒███    ▒███  ███     ▒▒███ ███     ▒▒███ ▒███  ███   ▒███    ▒▒▒
//  ▒███████████ ▒███      ▒███▒███      ▒███ ▒███████    ▒▒█████████
//  ▒███▒▒▒▒▒███ ▒███      ▒███▒███      ▒███ ▒███▒▒███    ▒▒▒▒▒▒▒▒███
//  ▒███    ▒███ ▒▒███     ███ ▒▒███     ███  ▒███ ▒▒███   ███    ▒███
//  █████   █████ ▒▒▒███████▒   ▒▒▒███████▒   █████ ▒▒████▒▒█████████
// ▒▒▒▒▒   ▒▒▒▒▒    ▒▒▒▒▒▒▒       ▒▒▒▒▒▒▒    ▒▒▒▒▒   ▒▒▒▒  ▒▒▒▒▒▒▒▒▒

#include <stdio.h>

static void print_cam(t_camera cam)
{
	printf("pos: %3i %3i %3i ", (int)cam.pos.x, (int)cam.pos.y, (int)cam.pos.z);
	printf("fov: %i \n", cam.fov);
	printf("up: %2f, %2f, %2f \n", cam.up.x, cam.up.y, cam.up.z);
	printf("forwd: %2f, %2f, %2f \n", cam.forward.x, cam.forward.y, cam.forward.z);
	printf("right: %2f, %2f, %2f \n", cam.right.x, cam.right.y, cam.right.z);
}

static int on_mouse_input(int keycode, int mouse_x, int mouse_y, t_app *app)
{
	(void)mouse_x;
	(void)mouse_y;
	(void)app;
	t_camera *cam = &app->scene->camera;
	t_vec3 delta;

	if (keycode == g_MOUSE_WHEEL_UP)
	{
		delta = vec3_mul(cam->forward, g_MOV_STRENGTH);
		cam->pos = vec3_add(cam->pos, delta);
	}
	else if (keycode == g_MOUSE_WHEEL_DN)
	{
		delta = vec3_mul(cam->forward, g_MOV_STRENGTH);
		cam->pos = vec3_sub(cam->pos, delta);
	}
	// else if (keycode == g_MOUSE_LEFT)
	// 	print info on the object being clicked on
	else
		printf("got %i key input \n", keycode);
	redraw(app);
	print_cam(*cam);
	return (0);
}

static int on_no_input(t_app *app)
{
	(void)app;
	return (0);
}

static int on_key_input(int keycode, t_app *app)
{
	t_scene *env = app->scene;
	t_camera *cam = &app->scene->camera;

	if (keycode == g_ESC_KEY)
		exit_n_clean(app);
	printf("got %i key input \n", keycode);

	t_vec3 delta;
	// Change cam orientation
	if (keycode == g_KEY_W)
		camera_pitch(cam, -g_ROT_STRENGTH);
	else if (keycode == g_KEY_S)
		camera_pitch(cam, g_ROT_STRENGTH);
	else if (keycode == g_KEY_A)
		camera_yaw(cam, -g_ROT_STRENGTH);
	else if (keycode == g_KEY_D)
		camera_yaw(cam, g_ROT_STRENGTH);

	// Change cam position
	else if (keycode == g_KEY_ARROW_UP)
	{
		delta = vec3_mul(cam->up, g_MOV_STRENGTH);
		cam->pos = vec3_add(cam->pos, delta);
	}
	else if (keycode == g_KEY_ARROW_DOWN)
	{
		delta = vec3_mul(cam->up, g_MOV_STRENGTH);
		cam->pos = vec3_sub(cam->pos, delta);
	}
	else if (keycode == g_KEY_ARROW_LEFT)
	{
		delta = vec3_mul(cam->right, g_MOV_STRENGTH * WINDOW_RATIO);
		cam->pos = vec3_sub(cam->pos, delta);
	}
	else if (keycode == g_KEY_ARROW_RIGHT)
	{
		delta = vec3_mul(cam->right, g_MOV_STRENGTH * WINDOW_RATIO);
		cam->pos = vec3_add(cam->pos, delta);
	}
	// FOV
	else if (keycode == g_NUMPAD_PLUS)
		cam->fov += 10;
	else if (keycode == g_NUMPAD_MINUS)
		cam->fov -= 10;
	print_cam(env->camera);
	redraw(app);
	return (0);
}

void hook_everything(t_app *app)
{
	mlx_key_hook(app->win, on_key_input, app);
	mlx_mouse_hook(app->win, on_mouse_input, app);
	mlx_loop_hook(app->mlx, on_no_input, app);
}
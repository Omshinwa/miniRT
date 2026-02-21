#include "../main.h"
#include "app.h"

// static int	parser(t_app *app, char* filename)
// {
// 	(void) filename;
// 	(void) app;

// 	return (0);
// }

// static const t_key g_keys_camera[] = {
//     { "pos",   T_VEC,  true,  offsetof(t_camera, pos) },
//     { "dir",   T_UNIT, true,  offsetof(t_camera, dir) },
//     { "fov",   T_FOV,  true,  offsetof(t_camera, fov) },
//     { NULL,    T_INVALID, false, 0 }
// };

int setup_scene(t_app *app, char *filename)
{
	(void)filename;

	// init scene
	app->scene = calloc(1, sizeof(t_scene));
	if (!app->scene)
		exit_n_clean(app);

	app->scene->global_cam = (t_camera){{0, 0, 0}, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, 70};

	app->scene->number_of_obj = 2; // lets put two spheres for now
	app->scene->objects = calloc(app->scene->number_of_obj + 1, sizeof(t_object));
	t_object *arr_obj = app->scene->objects;

	t_sphere s1 = {{0, 0, 100}, 10};
	t_object o1 = {OBJ_SPHERE, {s1}, {1, 0, 0}};
	t_sphere s2 = {{20, 0, 100}, 11};
	t_object o2 = {OBJ_SPHERE, {s2}, {0, 1, 1}};
	arr_obj[0] = o1;
	arr_obj[1] = o2;

	app->scene->ambient_light = (t_vec3){0, 0.1, 0};

	return (0);
}

// Z is forward
// X is right
// Y is up

// sphere
//   ^
//   |
//   z
// CAMERA x ---->
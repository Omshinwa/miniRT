#include "../main.h"
#include "app.h"

// static int	parser(t_app *app, char* filename)
// {
// 	(void) filename;
// 	(void) app;

// 	return (0);
// }

int setup_env3d(t_app *app, char* filename)
{
	(void) filename;

	// init env3d
	app->env3d = calloc(1, sizeof(t_env3d));
	if (!app->env3d)
		exit_n_clean(app);

	app->env3d->global_cam = (t_camera){{0, 0, 0}, {0, 0, 1}, {1, 0, 0}, {0, 1, 0}, 70};

	app->env3d->number_of_obj = 2; // lets put two spheres for now
	app->env3d->objects = calloc(app->env3d->number_of_obj + 1, sizeof(t_object));
	t_object *arr_obj = app->env3d->objects;

	t_sphere s1 = {{0, 0, 100}, 10};
	t_object o1 = {OBJ_SPHERE, {s1}, {1,0,0}};
	t_sphere s2 = {{20, 0, 100}, 11};
	t_object o2 = {OBJ_SPHERE, {s2}, {0,1,1}};
	arr_obj[0] = o1;
	arr_obj[1] = o2;

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
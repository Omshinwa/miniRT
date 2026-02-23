#include "../main.h"
#include "../app/app.h"
#include "parse.h"

// Your program must take as its first argument a scene description file with the .rt
// extension.
// ◦ Each type of element can be separated by one or more line breaks.
// ◦ Each type of information from an element can be separated by one or more
// spaces.
// ◦ Each type of element can be set in any order in the file.
// ◦ Elements defined by a capital letter can only be declared once in the scene.
// ◦ The first piece of information for each element is the type identifier (composed
// of one or two characters), followed by all specific information for each object
// in a strict order such as:

// Z is forward
// X is right
// Y is up

// sphere
//   ^
//   |
//   z
// CAMERA x ---->

int setup_scene(t_app *app, char *filename)
{
	(void)filename;

	// init scene
	app->scene = calloc(1, sizeof(t_scene));
	if (!app->scene)
		exit_n_clean(app);

	// This is the default camera, a camera at pos 0 with the default fov and
	// orientation vectors.
	app->scene->global_cam = (t_camera){{0, 0, 0},
										{0, 0, 1},
										{1, 0, 0},
										{0, 1, 0},
										70};

	// // eventually we will replace the code below with the parser:

	// app->scene->number_of_obj = 2; // lets put two spheres for now
	// app->scene->objects = calloc(app->scene->number_of_obj + 1, sizeof(t_object));
	// t_object *arr_obj = app->scene->objects;

	// t_sphere s1 = {{0, 0, 100}, 10};
	// t_object o1 = {OBJ_SPHERE, {s1}, {1, 0, 0}};
	// t_sphere s2 = {{20, 0, 100}, 11};
	// t_object o2 = {OBJ_SPHERE, {s2}, {0, 1, 1}};
	// arr_obj[0] = o1;
	// arr_obj[1] = o2;

	// app->scene->ambient_light = (t_vec3){0, 0.1, 0};

	if (parse_scene_file(app->scene, "test1.rt"))
		exit_n_clean(app);

	return (0);
}
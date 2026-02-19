#ifndef APP_H
#define APP_H

#include "../math/math.h"
#include "../../minilibx-linux/mlx.h"
#include <sys/time.h>

typedef struct s_list_container
{
	t_obj_type type;
	t_any_obj obj;
	struct s_object *next;
} t_list_container;

// Camera is defined by a bunch of stuff i dont understand
typedef struct s_camera
{
	t_vec3 pos;
	t_vec3 forward; // normalized direction vector
	t_vec3 right;	// perpendicular to forward & up
	t_vec3 up;		// camera's up direction
	int fov;		// in degrees
} t_camera;

typedef struct s_app
{
	// t_minilib minilib;
	void *mlx_ptr;
	void *win_ptr;
	void *img_ptr;
	char *first_pixel;
	int pixel_depth;
	int size_line;
	int endian;

	struct timeval time;

	t_camera global_cam;
	// pointeur a la camera

	// liste des objets a dessiner
	// t_node_obj

	// liste des lumieres

	// lumiere ambiante
} t_app;

// typedef struct s_minilib_interface
// {
// 	void *mlx_ptr;
// 	void *win_ptr;
// 	void *img_ptr;
// 	char *first_pixel;
// 	int pixel_depth;
// 	int size_line;
// 	int endian;

// } t_minilib;

extern t_sphere g_s;

int exit_n_clean(t_app *app);
t_app *create_app(void);

#endif
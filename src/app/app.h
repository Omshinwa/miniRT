#ifndef APP_H
#define APP_H

#include "../math/math.h"
#include "../../minilibx-linux/mlx.h"
#include <sys/time.h>

#include "../env3d/env3d.h"

typedef struct s_list_container
{
	t_obj_type type;
	t_any_obj obj;
	struct s_object *next;
} t_list_container;

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

	t_env3d *env3d;

	struct timeval time;
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

void hook_everything(t_app *app);

#endif
#ifndef APP_H
#define APP_H

#include "../math/math.h"
#include "../../minilibx-linux/mlx.h"
#include <sys/time.h>

#include "../scene/scene.h"

typedef struct s_app
{
	void *mlx_ptr;
	void *win_ptr;
	void *img_ptr;
	char *first_pixel;
	int pixel_depth;
	int size_line;
	int endian;

	t_scene *scene;

	struct timeval time;
} t_app;

int exit_n_clean(t_app *app);
t_app *create_app(void);

void hook_everything(t_app *app);
void redraw(t_app *app);

int setup_scene(t_app *app, char *filename);

#endif
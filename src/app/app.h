/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:15:21 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/18 06:36:31 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
#define APP_H

#include <sys/time.h>

#include "../common.h"
#include "../raytrace/scene_object.h"

#define RED "\033[31m"
#define RESET "\033[0m"

// Window size
#define WINDOW_X 800 // 1920
#define WINDOW_Y 600 // 1080

// Render context: MLX, window, image, scene and dimensions
typedef struct s_data {
    void *mlx;
    void *win;
    t_img img;
    t_scene scene;
    int width;
    int height;
} t_data;

// App state: display loop, progressive rendering, FPS etc..
typedef struct s_app {
    void *mlx;
    void *win;
    t_img img;
    t_scene *scene;
    t_object *target_object;
    int fps;
    struct timeval time;
} t_app;

// Camera rotation (pitch/yaw/roll) and position display
void rotate_around_axis(t_vec3 *v, t_vec3 k, float angle);
void camera_pitch(t_camera *cam, float angle);
void camera_yaw(t_camera *cam, float angle);
void camera_roll(t_camera *cam, float angle);

// Time management (FPS)
int update_time(t_app *app);

// App init, safe_calloc and cleanup
void exit_n_clean(t_app *app, char *msg);
void *safe_calloc(size_t nmemb, size_t size);
t_app *create_app(void);

// Input handling (keyboard, mouse, hooks)
int on_key_input(int keycode, t_app *app);
int on_mouse_input(int keycode, int mouse_x, int mouse_y, t_app *app);
void hook_everything(t_app *app);

// Image refresh
void redraw(t_app *app);
char *type_to_str(t_obj_type type);
void draw_info(t_app *app);

// Scene parsing and per-pixel ray tracing
int setup_scene(t_app *app, char *filename);
int calc_pixel_color(t_app *app, int x, int y);
void render_frame_multithreaded(t_app *app);
void destroy_thread_pool(void);

// Object selection by click (picking)
t_object *get_object_from_xy(t_app *app, int x, int y);

#endif

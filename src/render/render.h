#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "../math/math.h"
#include "../app/app.h"

#define MAX_DRAW_DISTANCE 99999

t_vec3 get_direction_vector(t_camera camera, int pixel_x, int pixel_y);

int calc_pixel_color(t_scene *scene, int x, int y);

#endif
#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "../math/math.h"
#include "../app/app.h"

#define MAX_DRAW_DISTANCE 99999

t_vec3 camera_pixel_to_vector(t_camera camera, int pixel_x, int pixel_y);

int calc_pixel_color(t_scene *scene, int x, int y);

// object render
float get_hit_sphere(t_vec3 origin, t_vec3 D, t_sphere sphere);

float get_hit_plane(t_vec3 ray_O, t_vec3 ray_D, t_vec3 pl_O, t_vec3 pl_N);
float get_hit_cylinder(t_vec3 origin, t_vec3 D, t_cylinder cylinder);

#endif
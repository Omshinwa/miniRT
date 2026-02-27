#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "../math/math.h"
#include "../app/app.h"
#include "../main.h"

static const int MAX_DRAW_DISTANCE = 99999;
static const int g_CHECKERBOARD_SCALE_FACTOR = 10;

int calc_pixel_color(t_app *app, int x, int y);

t_vec3 camera_pixel_to_vector(t_camera camera, int pixel_x, int pixel_y);
t_object *get_hit(t_scene *scene, t_vec3 origin, t_vec3 d_vector, float *closest_dist);

// object render
float get_hit_sphere(t_vec3 origin, t_vec3 D, t_sphere sphere);
float get_hit_plane(t_vec3 ray_O, t_vec3 ray_D, t_vec3 pl_O, t_vec3 pl_N);
float get_hit_cylinder(t_vec3 origin, t_vec3 D, t_cylinder cylinder);

// texture
void sphere_uv(t_vec3 P, t_sphere obj, float *u, float *v);
void plane_uv(t_vec3 P, t_plane obj, float *u, float *v);

#endif
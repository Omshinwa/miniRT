#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "../math/math.h"
#include "../app/app.h"
#include "../main.h"

typedef struct s_hit
{
	float		t;
	t_vec3		point;
	t_vec3		normal;
	t_vec3		color;
	int			hit;
	t_object	*obj;
}	t_hit;

// DIR is the normalized directional vector
typedef struct s_ray
{
	t_vec3 origin;
	t_vec3 dir;
} t_ray;


static const int MAX_DRAW_DISTANCE = 1e8;
static const int g_CHECKERBOARD_SCALE_FACTOR = 10;

int calc_pixel_color(t_app *app, int x, int y);

t_object *get_hit(t_scene *scene, t_vec3 origin, t_vec3 d_vector, float *closest_dist);


t_hit intersect_objects(t_scene *scene, t_vec3 origin, t_vec3 d_vector);

// object render
float get_hit_sphere(t_vec3 origin, t_vec3 D, t_sphere sphere);
float get_hit_plane(t_vec3 ray_O, t_vec3 ray_D, t_vec3 pl_O, t_vec3 pl_N);
float get_hit_cylinder(t_vec3 origin, t_vec3 D, t_cylinder cylinder);

// texture
void sphere_uv(t_vec3 P, t_sphere obj, float *u, float *v);
void plane_uv(t_vec3 P, t_plane obj, float *u, float *v);

#endif
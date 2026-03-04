#include "../main.h"
#include "render.h"

// Given a single object and a vector+origin
// Return the HIT point (-1 if none)
static float get_hit_object(t_vec3 origin, t_vec3 vector, t_object obj)
{
	float hit;

	hit = -1.0f;
	if (obj.type == OBJ_SPHERE)
		hit = get_hit_sphere(origin, vector, obj.data.sphere);
	else if (obj.type == OBJ_CYLINDER)
		hit = get_hit_cylinder(origin, vector, obj.data.cylinder);
	else if (obj.type == OBJ_PLANE)
		hit = get_hit_plane(origin, vector, obj.data.plane.pos, obj.data.plane.normal);
	else
		assert(0);
	// if PLANE, elif CYLINDER
	return (hit);
}

// given a ray (VECTOR + ORIGIN), and the environment (SCENE),
// goes through the list of objects and return:
// OBJ_HIT: the closest object hit (NULL if none)
// DISTANCE: the distance it hit at (through output parameter)
t_object *get_hit(t_scene *scene, t_vec3 origin, t_vec3 d_vector, float *closest_dist)
{
	t_object *obj_hit;
	float current_dist;
	int i = -1;

	obj_hit = NULL;
	*closest_dist = MAX_DRAW_DISTANCE;

	while (++i < scene->number_of_obj)
	{
		current_dist = get_hit_object(origin, d_vector, scene->objects[i]);
		if (current_dist > 0.0f && current_dist < *closest_dist)
		{
			*closest_dist = current_dist;
			obj_hit = &scene->objects[i];
		}
	}
	return (obj_hit);
}

t_vec3 vec3_negate(t_vec3 v)
{
	return (vec3_new(-v.x, -v.y, -v.z));
}


t_vec3	get_sphere_normal(t_sphere *sp, t_vec3 *point)
{
	return (vec3_normalize(vec3_sub(*point, sp->pos)));
}
t_vec3 get_plane_normal(t_plane *pl, t_ray *ray)
{
	if (vec3_dot(pl->normal, ray->dir) > 0)
		return (vec3_negate(pl->normal));
	return (pl->normal);
}


static t_vec3	get_normal(t_object *obj, t_vec3 *point, t_ray *ray)
{
	if (obj->type == OBJ_SPHERE)
		return (get_sphere_normal(&obj->data.sphere, point));
	if (obj->type == OBJ_PLANE)
		return (get_plane_normal(&obj->data.plane, ray));
	return ((t_vec3){0, 1, 0});
}

static void	fill_hit_info(t_hit *hit, t_ray *ray)
{
	hit->point = vec3_add(ray->origin, vec3_mul(ray->dir, hit->t));
	hit->normal = get_normal(hit->obj, &hit->point, ray);
	hit->color = hit->obj->color;
}

// return a t_hit.
// t_hit.obj = NULL if no hit
t_hit intersect_objects(t_scene *scene, t_vec3 origin, t_vec3 d_vector)
{
	t_hit hit;
	t_object *obj;
	float t;
	t_ray ray = (t_ray) {origin, d_vector};

	hit.obj = NULL;
	hit.hit = 0;
	hit.t = MAX_DRAW_DISTANCE;

	int i = -1;
	while (++i < scene->number_of_obj)
	{
		t = 0;
		if (get_hit_object(origin, d_vector, scene->objects[i]) > 0.0f && t < hit.t)
		{
			hit.t = t;
			hit.obj = obj;
		}
	}
	if (hit.obj)
		fill_hit_info(&hit, &ray);
	return (hit);
}
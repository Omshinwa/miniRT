#ifndef MATH_H
#define MATH_H

#include <math.h>

typedef struct s_vec3
{
	float x;
	float y;
	float z;
} t_vec3;

typedef struct s_sphere
{
	t_vec3 pos;
	float r;
} t_sphere;

typedef struct s_plane
{
	t_vec3 pos;
	t_vec3 normal;
} t_plane;

typedef struct s_cylinder
{
	t_vec3 pos;
	t_vec3 axis;
	float radius;
	float height;
} t_cylinder;

typedef union u_any_obj
{
	t_sphere sp;
	t_plane pl;
	t_cylinder cy;
} t_any_obj;

typedef enum e_obj_type
{
	OBJ_PLANE,
	OBJ_SPHERE,
	OBJ_CYLINDER,
} t_obj_type;

t_vec3 vec3_normalize(t_vec3 v);
float dot_product(t_vec3 a, t_vec3 b);
t_vec3 vec3_add_op(t_vec3 a, t_vec3 b);
t_vec3 vec3_scale_op(t_vec3 v, float s);
t_vec3 vec3_minus_op(t_vec3 a, t_vec3 b);
float vec3_length(t_vec3 v);

#endif
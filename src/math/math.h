#ifndef MATH_H
#define MATH_H

#include <float.h> // for FLT_MAX
#include <math.h>

// MATHS

float min_pos_f(float a, float b);
float quadratic_roots(float a, float b, float c);

// VECTORS

typedef struct s_vec3
{
	float x;
	float y;
	float z;
} t_vec3;

t_vec3 vec3_normalize(t_vec3 v);
float dot_product(t_vec3 a, t_vec3 b);
/* Cross product: a × b */
t_vec3 vec3_cross(t_vec3 a, t_vec3 b);

t_vec3 vec3_add(t_vec3 a, t_vec3 b);
t_vec3 vec3_scale(t_vec3 v, float s);
t_vec3 vec3_minus(t_vec3 a, t_vec3 b);
float vec3_length(t_vec3 v);

t_vec3 t_to_P(t_vec3 O, t_vec3 D, float t);

int to_color_int(t_vec3 c);
t_vec3 color_mult(t_vec3 a, t_vec3 b);
#endif
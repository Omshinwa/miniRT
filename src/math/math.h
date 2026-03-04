#ifndef MATH_H
#define MATH_H

#include <float.h> // for FLT_MAX
#include <math.h>

#include <assert.h> // remove for build

#define PI 3.14159265358979323846

// MATHS

float min_positive_f(float a, float b);
float quadratic_roots(float a, float b, float c);

// VECTORS

typedef struct s_vec3
{
	float x;
	float y;
	float z;
} t_vec3;

t_vec3 vec3_normalize(t_vec3 v);
float vec3_dot(t_vec3 a, t_vec3 b);
/* Cross product: a × b */
t_vec3 vec3_cross(t_vec3 a, t_vec3 b);

t_vec3 vec3_add(t_vec3 a, t_vec3 b);
t_vec3 vec3_mul(t_vec3 v, float s);
t_vec3 vec3_sub(t_vec3 a, t_vec3 b);
float vec3_len(t_vec3 v);

t_vec3 t_to_P(t_vec3 O, t_vec3 D, float t);

int to_color_int(t_vec3 c);
t_vec3 color_mult(t_vec3 a, t_vec3 b);
#endif
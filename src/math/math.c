#include "math.h"

//  ██████   ██████   █████████   ███████████ █████   █████
// ░░██████ ██████   ███░░░░░███ ░█░░░███░░░█░░███   ░░███
//  ░███░█████░███  ░███    ░███ ░   ░███  ░  ░███    ░███   █████
//  ░███░░███ ░███  ░███████████     ░███     ░███████████  ███░░
//  ░███ ░░░  ░███  ░███░░░░░███     ░███     ░███░░░░░███ ░░█████
//  ░███      ░███  ░███    ░███     ░███     ░███    ░███  ░░░░███
//  █████     █████ █████   █████    █████    █████   █████ ██████
// ░░░░░     ░░░░░ ░░░░░   ░░░░░    ░░░░░    ░░░░░   ░░░░░ ░░░░░░

// given two floats, return the smallest positive number between a and b
// return -1 if they're both negative
float min_pos_f(float a, float b)
{
	float min;
	if (a < 0 && b < 0)
		return (-1.0f);
	min = FLT_MAX;
	if (a > 0.0f)
		min = a;
	if (b > 0.0f && b < min)
		min = b;
	return (min);
}

// This solves a quadradratic equation in the form at² + bt + c = 0
// currently returns only the smallest positive root,
// -1 if None
// maybe should return both roots root1 and root2?
// returns -1, -1 if no solution in R
float quadratic_roots(float a, float b, float c)
{
	float d;
	float sqrt_d;
	float t0;
	float t1;
	d = b * b - 4.0f * a * c;
	// If Δ<0: no intersection
	// If Δ=0: one intersection (tangent)
	// If Δ>0: two intersections
	if (d < 0.0f)
		return -1.0f;
	sqrt_d = sqrtf(d);
	t0 = (-b - sqrt_d) / (2.0f * a);
	t1 = (-b + sqrt_d) / (2.0f * a);

	return min_pos_f(t0, t1);
}

//  █████   █████ ██████████   █████████  ███████████  ████████
// ▒▒███   ▒▒███ ▒▒███▒▒▒▒▒█  ███▒▒▒▒▒███▒█▒▒▒███▒▒▒█ ███▒▒▒▒███
//  ▒███    ▒███  ▒███  █ ▒  ███     ▒▒▒ ▒   ▒███  ▒ ▒▒▒    ▒███
//  ▒███    ▒███  ▒██████   ▒███             ▒███       ██████▒
//  ▒▒███   ███   ▒███▒▒█   ▒███             ▒███      ▒▒▒▒▒▒███
//   ▒▒▒█████▒    ▒███ ▒   █▒▒███     ███    ▒███     ███   ▒███
//     ▒▒███      ██████████ ▒▒█████████     █████   ▒▒████████
//      ▒▒▒      ▒▒▒▒▒▒▒▒▒▒   ▒▒▒▒▒▒▒▒▒     ▒▒▒▒▒     ▒▒▒▒▒▒▒▒

// Returns the dot product of two vectors
float dot_product(t_vec3 a, t_vec3 b) { return (a.x * b.x + a.y * b.y + a.z * b.z); }
// Cross product: A × B
// The cross product returns the vector that is normal to the plane defined by a and b
t_vec3 vec3_cross(t_vec3 a, t_vec3 b)
{
	return (t_vec3){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x};
}

// A + B
t_vec3 vec3_add(t_vec3 a, t_vec3 b) { return (t_vec3){a.x + b.x, a.y + b.y, a.z + b.z}; }
// V*s
t_vec3 vec3_scale(t_vec3 v, float s) { return (t_vec3){v.x * s, v.y * s, v.z * s}; }
// A - B
t_vec3 vec3_minus(t_vec3 a, t_vec3 b) { return ((t_vec3){a.x - b.x, a.y - b.y, a.z - b.z}); }
// uses srqt
float vec3_length(t_vec3 v) { return sqrtf(dot_product(v, v)); }
// Return normalized V, uses sqrt
t_vec3 vec3_normalize(t_vec3 v)
{
	float l = vec3_length(v);
	if (l == 0.0f)
		return v;
	return vec3_scale(v, 1.0f / l);
}

//    █████████     ███████    █████          ███████    ███████████
//   ███▒▒▒▒▒███  ███▒▒▒▒▒███ ▒▒███         ███▒▒▒▒▒███ ▒▒███▒▒▒▒▒███
//  ███     ▒▒▒  ███     ▒▒███ ▒███        ███     ▒▒███ ▒███    ▒███
// ▒███         ▒███      ▒███ ▒███       ▒███      ▒███ ▒██████████
// ▒███         ▒███      ▒███ ▒███       ▒███      ▒███ ▒███▒▒▒▒▒███
// ▒▒███     ███▒▒███     ███  ▒███      █▒▒███     ███  ▒███    ▒███
//  ▒▒█████████  ▒▒▒███████▒   ███████████ ▒▒▒███████▒   █████   █████
//   ▒▒▒▒▒▒▒▒▒     ▒▒▒▒▒▒▒    ▒▒▒▒▒▒▒▒▒▒▒    ▒▒▒▒▒▒▒    ▒▒▒▒▒   ▒▒▒▒▒

int to_color_int(t_vec3 c)
{
	int red;
	int green;
	int blue;
	int color;

	red = fminf(255 * c.x, 255);
	green = fminf(255 * c.y, 255);
	blue = fminf(255 * c.z, 255);

	color = (red << 16) | (green << 8) | blue;
	return (color);
}

t_vec3 color_mult(t_vec3 a, t_vec3 b) { return (t_vec3){a.x * b.x, a.y * b.y, a.z * b.z}; }
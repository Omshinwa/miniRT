//  ██████   ██████   █████████   ███████████ █████   █████
// ░░██████ ██████   ███░░░░░███ ░█░░░███░░░█░░███   ░░███
//  ░███░█████░███  ░███    ░███ ░   ░███  ░  ░███    ░███   █████
//  ░███░░███ ░███  ░███████████     ░███     ░███████████  ███░░
//  ░███ ░░░  ░███  ░███░░░░░███     ░███     ░███░░░░░███ ░░█████
//  ░███      ░███  ░███    ░███     ░███     ░███    ░███  ░░░░███
//  █████     █████ █████   █████    █████    █████   █████ ██████
// ░░░░░     ░░░░░ ░░░░░   ░░░░░    ░░░░░    ░░░░░   ░░░░░ ░░░░░░

#include "math.h"

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
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
// A + B
t_vec3 vec3_add_op(t_vec3 a, t_vec3 b) { return (t_vec3){a.x + b.x, a.y + b.y, a.z + b.z}; }
// V*s
t_vec3 vec3_scale_op(t_vec3 v, float s) { return (t_vec3){v.x * s, v.y * s, v.z * s}; }
// A - B
t_vec3 vec3_minus_op(t_vec3 a, t_vec3 b) { return ((t_vec3){a.x - b.x, a.y - b.y, a.z - b.z}); }
// uses srqt
float vec3_length(t_vec3 v) { return sqrtf(dot_product(v, v)); }
// Return normalized V, uses sqrt
t_vec3 vec3_normalize(t_vec3 v)
{
	float l = vec3_length(v);
	if (l == 0.0f)
		return v;
	return vec3_scale_op(v, 1.0f / l);
}
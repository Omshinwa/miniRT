#include "render.h"

// ray_O : ray origin point
// D : normalized directional vector of the ray
// pl_P : a point on the plane
// N : normalized vector normal to the plane
// returns -1 if it's parallel to the plane
float get_hit_plane(t_vec3 ray_O, t_vec3 D, t_vec3 pl_P, t_vec3 N)
{
	float t;

	float denom = dot_product(D, N);
	// Optional safety check (ray parallel to plane)
	if (fabs(denom) < 1e-6)
		return (-1.0f); // or handle differently

	t = dot_product(vec3_minus(pl_P, ray_O), N);
	t /= dot_product(D, N);
	return (t);
}
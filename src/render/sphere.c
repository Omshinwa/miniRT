#include "render.h"

// Returns the nearest collision point t, or -1 if None
float get_hit_sphere(t_vec3 origin, t_vec3 D, t_sphere sphere)
{
	// L est le vecteur du centre de la sphere a la camera
	// L = O - C
	t_vec3 L = vec3_minus(origin, sphere.pos);
	float a = dot_product(D, D);
	float b = 2.0f * dot_product(D, L);
	float c = dot_product(L, L) - sphere.radius * sphere.radius;

	return quadratic_roots(a, b, c);
}

// From P, a point on the sphere obj, return UV
// (u,v) ∈ [0,1]
void sphere_uv(t_vec3 P, t_sphere obj, float *u, float *v)
{
	t_vec3 local;
	local = vec3_normalize(vec3_minus(P, obj.pos));
	// local is the normalized vector from sphere center to P.
	*u = 0.5 + atan2(local.z, local.x) / (2 * PI);
	*v = 0.5 - asin(local.y) / PI;
}

// We should probably only calculate UV once per obj
// Then we do
// Checkerboard
// + texture
// + bump map
#include "render.h"

// O : ray origin point
// D : normalized directional vector of the ray
// p0 : a point on the plane
// N : normalized vector normal to the plane
// returns -1 if it's parallel to the plane
float get_hit_plane(t_vec3 O, t_vec3 D, t_vec3 p0, t_vec3 N)
{
	float t;

	float denom = dot_product(D, N);
	// Optional safety check (ray parallel to plane)
	if (fabs(denom) < 1e-6)
		return (-1.0f); // or handle differently

	t = dot_product(vec3_minus(p0, O), N);
	t /= dot_product(D, N);
	return (t);
}

// From P, a point on the sphere obj, return UV
// (u,v) ∈ [0,1] in a repeating manner for tiling
void plane_uv(t_vec3 P, t_plane obj, float *u, float *v)
{
	// create a new 3d repere orthonormee with Z = obj.axis

	t_vec3 tmp = {1, 0, 0};
	// If the normal is too close to our arbitrary X, use Y instead to avoid a zero-length cross product
	if (fabs(dot_product(tmp, obj.normal)) > 0.99)
		tmp = (t_vec3){0, 1, 0};

	// 1. Create Y_axis: orthogonal to both the normal and our tmp vector
	// This is guaranteed to be ON THE PLANE.
	t_vec3 Y_axis = vec3_normalize(vec3_cross(obj.normal, tmp));

	// 2. Create X_axis: orthogonal to both the normal and Y_axis
	// This is also guaranteed to be ON THE PLANE.
	t_vec3 X_axis = vec3_cross(Y_axis, obj.normal);

	t_vec3 CP; // this is the vector from the center of the plane to the point.
	CP = vec3_minus(P, obj.pos);
	*u = dot_product(CP, X_axis);
	*v = dot_product(CP, Y_axis);

	// // (u,v) ∈ [0,1]
	*u /= g_CHECKERBOARD_SCALE_FACTOR * g_CHECKERBOARD_SCALE_FACTOR;
	*v /= g_CHECKERBOARD_SCALE_FACTOR * g_CHECKERBOARD_SCALE_FACTOR;
	*u -= floor(*u);
	*v -= floor(*v);
}

// int is_checkerboard_plane(t_vec3 P, t_plane obj)
// {
// 	// create a new 3d repere orthonormee with Z = obj.axis
// 	t_vec3 X_axis = {1, 0, 0};
// 	// check if it's not parallel
// 	if (dot_product(X_axis, obj.normal) > 0.999)
// 		X_axis = (t_vec3){0, 1, 0};
// 	t_vec3 Y_axis = vec3_cross(obj.normal, X_axis);

// 	t_vec3 CP; // this is the vector from the center of the plane to the point.
// 	CP = vec3_minus(P, obj.pos);
// 	int u = dot_product(CP, X_axis);
// 	int v = dot_product(CP, Y_axis);

// 	u = floor((float)u / g_SCALE_FACTOR);
// 	v = floor((float)v / g_SCALE_FACTOR);
// 	if ((u + v) % 2 == 0)
// 		return 1;
// 	else
// 		return 0;
// }

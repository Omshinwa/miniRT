#include "render.h"

// returns a positive number if it hits a disk:
// O is the origin of the ray, D the normalized directional vector
// C is the center of the disk
// R is it's radius
// N is the normal vector of the disk
static float get_hit_disk(t_vec3 O, t_vec3 D, t_vec3 C, t_vec3 N, float R)
{
	float t;

	t = get_hit_plane(O, D, C, N);
	if (t < 0)
		return (t);
	// next we check if it's inside the radius
	t_vec3 P;
	P = t_to_P(O, D, t);
	t_vec3 CP;
	CP = vec3_minus(P, C);
	if (dot_product(CP, CP) <= R * R)
		return (t + 30); // TODO remove it later, this is just to color the disk differently
	return (-1.0f);
}

// 3 Steps
// 1. calculate if the ray hits an infinite cylinder
// 2. check if it's inside the limited cylinder
// 3. check if it hits the caps
// 4. returns the closest t
float get_hit_cylinder(t_vec3 O, t_vec3 D, t_cylinder cylinder)
{
	// let's calculate D_perp = D - dot(D, A) * A
	t_vec3 D_perp;
	t_vec3 D_parallel;
	D_parallel = vec3_scale(cylinder.axis, dot_product(D, cylinder.axis));
	// this is the parallel component of D onto A
	D_perp = vec3_minus(D, D_parallel);
	// because D = D_perp + D_parallel

	// OC_perp = OC - dot(OC, A) * A
	t_vec3 OC;
	t_vec3 OC_parallel;
	t_vec3 OC_perp;
	OC = vec3_minus(O, cylinder.pos);
	OC_parallel = vec3_scale(cylinder.axis, dot_product(OC, cylinder.axis));
	OC_perp = vec3_minus(OC, OC_parallel);

	// Now we solve:
	// | D_perp * t + OC_perp |² = r²
	// |
	// a = dot(D_perp, D_perp)
	// b = 2 * dot(D_perp, OC_perp)
	// c = dot(OC_perp, OC_perp) - r²
	float t = quadratic_roots(
		dot_product(D_perp, D_perp),
		2 * dot_product(D_perp, OC_perp),
		dot_product(OC_perp, OC_perp) - cylinder.radius * cylinder.radius);

	// t is the intersection with the infinite cylinder

	// Let's calculate P, the intersection point.
	t_vec3 P = t_to_P(O, D, t);
	// y is the Cylinder.axis component of P
	float y = dot_product(vec3_minus(P, cylinder.pos), cylinder.axis);

	// the point is outside the cylinder on top and bottom if:
	if (y < -cylinder.height / 2 || y > cylinder.height / 2)
		t = -1.0f;

	// Check intersection with caps
	// point Bottom center:
	// 	Cb = C - A * (h/2)
	t_vec3 Cb = vec3_minus(cylinder.pos, vec3_scale(cylinder.axis, cylinder.height / 2));
	// point Top center:
	// Ct = C + A * (h/2)
	t_vec3 Ct = vec3_add(cylinder.pos, vec3_scale(cylinder.axis, cylinder.height / 2));

	float t1 = get_hit_disk(O, D, Cb, cylinder.axis, cylinder.radius);
	float t2 = get_hit_disk(O, D, Ct, cylinder.axis, cylinder.radius);

	t1 = min_pos_f(t1, t2);
	// 	Check if the point is inside the disk
	// If true → the ray hits the cap
	// If false → it misses the cap
	// Then check if point lies inside radius:
	// |P - Ccap| ≤ r

	return (min_pos_f(t, t1));
}
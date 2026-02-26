#include "render.h"

float get_hit_cylinder(t_vec3 origin, t_vec3 D, t_cylinder cylinder)
{
	// methode 1
	// commencons par creer un nouveau repere orthonorme avec le centre du cylindre comme origine
	// (0,0,0) avec l'axe du cylindre comme 3e vecteur de la base orthonormee
	// on recalcule le rayon dans cette nouvelle base. et on resout sans se soucier du Z.

	// methode 2

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
	OC = vec3_minus(origin, cylinder.pos);
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
	t_vec3 P = vec3_add(origin, vec3_scale(D, t));
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
	(void)Cb;
	(void)Ct;
	// Intersect ray with plane:
	// dot(D, A) ≠ 0
	// (P−Ccap​)⋅A=0
	// Equation d'un plan defini avec A (vecteur normal) et C (le point centre)
	// (P - C) . A = 0
	// Substitute P with P = O + D * t
	// (O + D * t - C) . A = 0
	// solve for t: (we check if it's positive, if the plane is in front of the camera)
	// t = (C - O).A / (D.A)
	// Important: If D·A ≈ 0, the ray is parallel to the plane, so there is no intersection with this cap.
	// 	Check if the point is inside the disk

	// The plane is infinite; the cap is a disk of radius r.

	// Compute the intersection point:

	// P=O+tD
	// P=O+tD

	// Check distance to the cap center:

	// ∣P−Ccap∣≤r
	// ∣P−C
	// cap
	// 	​

	// ∣≤r

	// If true → the ray hits the cap

	// If false → it misses the cap
	// t = dot_product(Ccap)
	// Solve:
	// t = dot(Ccap - O, A) / dot(D, A)
	// Then check if point lies inside radius:
	// |P - Ccap| ≤ r

	return (t);
}
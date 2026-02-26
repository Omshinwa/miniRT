#include "render.h"

// Equation d'un plan defini avec le vecteur normal (pl_N)) et plane_origin (pl_O)
// (P - pl_O) . pl_N = 0
// Substitute P with Point = ray_ + ray_D * t
// (ray_O + ray_D * t - pl_O) . pl_N = 0
// solve for t: (we check if it's positive, if the plane is in front of the camera)
// t = (pl_O - ray_O).pl_N / (ray_D.pl_N)
float get_hit_plane(t_vec3 ray_O, t_vec3 ray_D, t_vec3 pl_O, t_vec3 pl_N)
{
	float t;
	t = dot_product(vec3_minus(pl_O, ray_O), pl_N);
	t /= dot_product(ray_D, pl_N);
	return (t);
}

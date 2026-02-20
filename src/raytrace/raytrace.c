#include "../main.h"
#include "raytrace.h"

// Given a camera and a pixel position, returns D
// D is the normalized directional vector from the camera to the pixel at z=1
t_vec3 get_direction_vector(t_camera camera, int pixel_x, int pixel_y)
{
	// Build ray for pixel (x,y) in camera space using FOV
	// + 0.5f mean it's the center of the pixel
	float ndc_x = (2.0f * (pixel_x + 0.5f) / (float)WINDOW_X) - 1.0f; // -1..1
	float ndc_y = 1.0f - (2.0f * (pixel_y + 0.5f) / (float)WINDOW_Y); // 1..-1
	// (flip y), we do this because screen's Y axis points down
	// but the camera / math Y axis points up.
	//
	// window screen:
	//
	//	  (0,0) ─────────▶ x
	// 		│
	// 		│
	// 		▼
	// 		y
	//
	// camera / world / math space:
	//
	//		+Y (up)
	//		  ▲
	//		  |
	//		  |
	//		  └────▶ +X
	//		 /
	//		/
	// +Z (forward)
	//

	float aspect = (float)WINDOW_X / (float)WINDOW_Y;

	// Correct implementation of FOV -> scaling for rays, though i dont
	// really understand it. TODO
	float fov_rad = camera.fov * (PI / 180.0f);
	float scale = tanf(fov_rad * 0.5f);
	// My naive implementation
	// float scale = (float)app->global_cam.fov/100;

	/* Point on the image plane at z = 1 in camera space */
	float px = ndc_x * aspect * scale;
	float py = ndc_y * scale;

	// Rotate the points so that it matches the rotation of the camera.
	// Ray direction in world space: px*right + py*up + pz*forward
	// pz = 1 here:
	t_vec3 D = (t_vec3){
		px * camera.right.x + py * camera.up.x + camera.forward.x,
		px * camera.right.y + py * camera.up.y + camera.forward.y,
		px * camera.right.z + py * camera.up.z + camera.forward.z};

	/* normalize D */
	return (vec3_normalize(D));
}

// Returns the nearest collision point t, or -1 if None
float intersect_sphere(t_vec3 origin, t_vec3 D, t_sphere sphere)
{
	// L est le vecteur du centre de la sphere a la camera
	// L = O - C
	t_vec3 L = vec3_minus(origin, sphere.pos);
	float a = dot_product(D, D);
	float b = 2.0f * dot_product(D, L);
	float c = dot_product(L, L) - sphere.r * sphere.r;
	float disc = b * b - 4.0f * a * c;
	// If Δ<0: no intersection
	// If Δ=0: one intersection (tangent)
	// If Δ>0: two intersections
	if (disc < 0.0f)
		return -1.0f;
	float sqrt_d = sqrtf(disc);
	float t0 = (-b - sqrt_d) / (2.0f * a);
	float t1 = (-b + sqrt_d) / (2.0f * a);
	/* choose nearest positive t */
	float t = -1.0f;
	if (t0 > 0.0f)
		t = t0;
	if (t1 > 0.0f && (t < 0.0f || t1 < t))
		t = t1;
	return t;
}

static float get_intersection(t_camera cam, t_object obj, int x, int y)
{
	float	intersection;
	
	intersection = -1.0f;
	if (obj.type == OBJ_SPHERE)
		intersection = intersect_sphere(cam.pos, get_direction_vector(cam, x, y), obj.data.sphere);
	// if PLANE, elif CYLINDER
	return (intersection);
}

// return the Color for a peculiar (x, y) pixel
int calc_raytrace(t_scene *env, int x, int y)
{	
	int	i;
	t_object closest_obj;
	float	best_intersex;
	float	curr_intersex;
	t_camera cam;
	
	cam = env->global_cam;
	i = -1;
	best_intersex = 9999;
	while(++i < env->number_of_obj)
	{
		curr_intersex = get_intersection(cam, env->objects[i], x, y);
		if (curr_intersex > 0 && curr_intersex < best_intersex) // found better
		{
			closest_obj = env->objects[i];
			best_intersex = curr_intersex;
		}
	}

	t_vec3 color;
	
	color = (t_vec3){0,0,0};
	float distance = fmaxf(0, 255.0f - best_intersex);
	color = (t_vec3){distance/255.0f, distance/255.0f, distance/255.0f}; // gray
	color = color_mult(color, closest_obj.color);

	return (to_color(color));
}
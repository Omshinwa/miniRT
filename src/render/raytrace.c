#include "../main.h"
#include "render.h"

// Given a camera and a pixel position:
// returns the normalized directional vector from the camera to the pixel at z=1
t_vec3 camera_pixel_to_vector(t_camera camera, int pixel_x, int pixel_y)
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

// Given a single object and a vector+origin
// Return the HIT point (-1 if none)
static float get_hit_object(t_vec3 origin, t_vec3 vector, t_object obj)
{
	float hit;

	hit = -1.0f;
	if (obj.type == OBJ_SPHERE)
		hit = get_hit_sphere(origin, vector, obj.data.sphere);
	else if (obj.type == OBJ_CYLINDER)
		hit = get_hit_cylinder(origin, vector, obj.data.cylinder);
	else if (obj.type == OBJ_PLANE)
		hit = get_hit_plane(origin, vector, obj.data.plane.pos, obj.data.plane.normal);
	else
		assert(0);
	// if PLANE, elif CYLINDER
	return (hit);
}

// given a ray (VECTOR + ORIGIN), and the environment (SCENE),
// goes through the list of objects and return:
// OBJ_HIT: the closest object hit (NULL if none)
// DISTANCE: the distance it hit at (through output parameter)
t_object *get_hit(t_scene *scene, t_vec3 origin, t_vec3 d_vector, float *closest_dist)
{
	t_object *obj_hit;
	float current_dist;
	int i = -1;

	obj_hit = NULL;
	*closest_dist = MAX_DRAW_DISTANCE;

	while (++i < scene->number_of_obj)
	{
		current_dist = get_hit_object(origin, d_vector, scene->objects[i]);
		if (current_dist > 0.0f && current_dist < *closest_dist)
		{
			*closest_dist = current_dist;
			obj_hit = &scene->objects[i];
		}
	}
	return (obj_hit);
}
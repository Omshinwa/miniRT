// Camera rotation helpers

#include "../main.h"
#include "scene.h"

// Given a camera and a pixel position:
// returns the normalized directional vector from the camera to the pixel (at z=1 in cam space)
t_vec3 camera_pixel_to_vector(t_camera camera, int pixel_x, int pixel_y)
{
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

	// We need to know how much does the virtual screen stretches.
	// Knowing the FOV (field of view).
	float aspect = (float)WINDOW_X / (float)WINDOW_Y;
	float fov_rad = (camera.fov / 2) * (2.0f * PI / 360.0f); // converting to rad
	float scale_of_virtual_screen = tanf(fov_rad);

	/* Point on the image plane in camera space */
	float px = ndc_x * aspect * scale_of_virtual_screen;
	float py = ndc_y * scale_of_virtual_screen;
	float pz = 1;

	// Rotate the points so that it matches the rotation of the camera.
	// Ray direction in world space: px*right + py*up + pz*forward
	// pz = 1 here:
	t_vec3 D = (t_vec3){
		px * camera.right.x + py * camera.up.x + pz * camera.forward.x,
		px * camera.right.y + py * camera.up.y + pz * camera.forward.y,
		px * camera.right.z + py * camera.up.z + pz * camera.forward.z};

	/* normalize D */
	return (vec3_normalize(D));
}

// rotate vector v around unit axis k by `angle` (Rodrigues' rotation)
static void rotate_around_axis(t_vec3 *v, t_vec3 k, float angle)
{
	k = vec3_normalize(k); // K is always a unit vector but hey
	float c = cosf(angle);
	float s = sinf(angle);
	// Each terms are one component, eg if we rotate Z to the right:
	// term1 is Z (0.99)
	// term2 is X (0.04)
	// term3 is Y (0)
	// vector forward (0, 0, 1) -> (press D) -> (0.04, 0, 0.99)
	t_vec3 term1 = vec3_mul(*v, c);
	t_vec3 term2 = vec3_mul(vec3_cross(k, *v), s);
	float kdotv = vec3_dot(k, *v);
	t_vec3 term3 = vec3_mul(k, kdotv * (1.0f - c));
	*v = vec3_add(vec3_add(term1, term2), term3);
}

// look up/down — rotate around the camera's right axis
void camera_pitch(t_camera *cam, float angle)
{
	rotate_around_axis(&cam->forward, cam->right, angle);
	rotate_around_axis(&cam->up, cam->right, angle);

	cam->forward = vec3_normalize(cam->forward);
	cam->up = vec3_normalize(cam->up);
	cam->right = vec3_normalize(vec3_cross(cam->up, cam->forward));
}

// look left/right — rotate around the camera's up axis
void camera_yaw(t_camera *cam, float angle)
{
	t_vec3 axis = cam->up;
	rotate_around_axis(&cam->forward, axis, angle);
	rotate_around_axis(&cam->right, axis, angle);

	cam->forward = vec3_normalize(cam->forward);
	cam->right = vec3_normalize(cam->right);
	cam->up = vec3_normalize(vec3_cross(cam->forward, cam->right));
}

// tilt sideways (roll) — rotate around the camera's forward axis
void camera_roll(t_camera *cam, float angle)
{
	t_vec3 axis = cam->forward;
	rotate_around_axis(&cam->right, axis, angle);
	rotate_around_axis(&cam->up, axis, angle);

	cam->right = vec3_normalize(cam->right);
	cam->up = vec3_normalize(cam->up);
	cam->forward = vec3_normalize(vec3_cross(cam->right, cam->up));
}

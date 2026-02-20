// Camera rotation helpers

#include "../main.h"
#include "scene.h"

// rotate vector v around unit axis k by `angle` (Rodrigues' rotation)
static void rotate_around_axis(t_vec3 *v, t_vec3 k, float angle)
{
    k = vec3_normalize(k); // K is always a unit vector but hey
	float c = cosf(angle);
	float s = sinf(angle);
	t_vec3 term1 = vec3_scale(*v, c);
	t_vec3 term2 = vec3_scale(vec3_cross(k, *v), s);
	float kdotv = dot_product(k, *v);
	t_vec3 term3 = vec3_scale(k, kdotv * (1.0f - c));
	*v = vec3_add(vec3_add(term1, term2), term3);
}

// look up/down — rotate around the camera's right axis
void camera_pitch(t_camera *cam, float angle)
{
    rotate_around_axis(&cam->forward, cam->right, angle);
    rotate_around_axis(&cam->up,      cam->right, angle);

    cam->forward = vec3_normalize(cam->forward);
    cam->up      = vec3_normalize(cam->up);
    cam->right   = vec3_normalize(vec3_cross(cam->up, cam->forward));
}

// look left/right — rotate around the camera's up axis
void camera_yaw(t_camera *cam, float angle)
{
	t_vec3 axis = cam->up;
	rotate_around_axis(&cam->forward, axis, angle);
	rotate_around_axis(&cam->right	, axis, angle);

    cam->forward = vec3_normalize(cam->forward);
    cam->right   = vec3_normalize(cam->right);
    cam->up      = vec3_normalize(vec3_cross(cam->forward, cam->right));
}

// tilt sideways (roll) — rotate around the camera's forward axis
void camera_roll(t_camera *cam, float angle)
{
	t_vec3 axis = cam->forward;
	rotate_around_axis(&cam->right	, axis, angle);
	rotate_around_axis(&cam->up		, axis, angle);

    cam->right   = vec3_normalize(cam->right);
    cam->up      = vec3_normalize(cam->up);
    cam->forward = vec3_normalize(vec3_cross(cam->right, cam->up));
}

// Camera rotation helpers

#include "../main.h"
#include "env3d.h"

// rotate vector v around unit axis k by `angle` (Rodrigues' rotation)
static t_vec3 rotate_around_axis(t_vec3 v, t_vec3 k, float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    t_vec3 term1 = vec3_scale_op(v, c);
    t_vec3 term2 = vec3_scale_op(vec3_cross(k, v), s);
    float kdotv = dot_product(k, v);
    t_vec3 term3 = vec3_scale_op(k, kdotv * (1.0f - c));
    return vec3_add_op(vec3_add_op(term1, term2), term3);
}

// look up/down — rotate around the camera's right axis
void camera_pitch(t_camera *cam, float angle)
{
    t_vec3 axis = vec3_normalize(cam->right);
    cam->forward = vec3_normalize(rotate_around_axis(cam->forward, axis, angle));
    cam->up = vec3_normalize(rotate_around_axis(cam->up, axis, angle));

    /* re-orthonormalize basis to avoid drift */
    cam->right = vec3_normalize(vec3_cross(cam->up, cam->forward));
    cam->up = vec3_normalize(vec3_cross(cam->forward, cam->right));
}

// look left/right — rotate around the camera's up axis
void camera_yaw(t_camera *cam, float angle)
{
    t_vec3 axis = vec3_normalize(cam->up);
    cam->forward = vec3_normalize(rotate_around_axis(cam->forward, axis, angle));
    cam->right = vec3_normalize(rotate_around_axis(cam->right, axis, angle));

    /* re-orthonormalize basis to avoid drift */
    cam->up = vec3_normalize(vec3_cross(cam->forward, cam->right));
}

// tilt sideways (roll) — rotate around the camera's forward axis
void camera_roll(t_camera *cam, float angle)
{
    t_vec3 axis = vec3_normalize(cam->forward);
    cam->right = vec3_normalize(rotate_around_axis(cam->right, axis, angle));
    cam->up = vec3_normalize(rotate_around_axis(cam->up, axis, angle));

    /* re-orthonormalize basis to avoid drift */
    cam->right = vec3_normalize(vec3_cross(cam->up, cam->forward));
    cam->up = vec3_normalize(vec3_cross(cam->forward, cam->right));
}

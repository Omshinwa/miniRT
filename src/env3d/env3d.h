#ifndef ENV3D_H
#define ENV3D_H

#include "../math/math.h"

typedef struct s_list_container
{
	t_obj_type type;
	t_any_obj obj;
	struct s_object *next;
} t_list_container;

typedef struct s_camera
{
	t_vec3 pos;
	t_vec3 forward; // normalized direction vector
	t_vec3 right;	// perpendicular to forward & up
	t_vec3 up;		// camera's up direction
	int fov;		// in degrees
} t_camera;

typedef struct s_env3d
{
	t_camera global_cam;
	// pointeur a la camera

	// liste des objets a dessiner
	// t_node_obj

	// liste des lumieres

	// lumiere ambiante

} t_env3d;

void camera_pitch(t_camera *cam, float angle);
void camera_yaw(t_camera *cam, float angle);
void camera_roll(t_camera *cam, float angle);

#endif
#ifndef scene_H
#define scene_H

#include "../math/math.h"

// 3D objects

typedef enum e_obj_type
{
	OBJ_PLANE,
	OBJ_SPHERE,
	OBJ_CYLINDER,
} t_obj_type;

typedef struct s_sphere
{
	t_vec3 pos;
	float r;
} t_sphere;

typedef struct s_plane
{
	t_vec3 pos;
	t_vec3 normal;
} t_plane;

typedef struct s_cylinder
{
	t_vec3 pos;
	t_vec3 axis;
	float radius;
	float height;
} t_cylinder;

typedef union u_any_obj
{
	t_sphere	sphere;
	t_plane		plane;
	t_cylinder	cylinder;
} t_any_obj;

typedef struct s_object
{
	t_obj_type 		type;
	t_any_obj		data;
	t_vec3 color;
} t_object;

// lights

typedef struct s_light
{
	t_vec3 pos;
	float brightness;
	t_vec3 color;
} t_light;

typedef struct s_camera
{
	t_vec3 pos;
	t_vec3 forward; // normalized direction vector
	t_vec3 right;	// perpendicular to forward & up
	t_vec3 up;		// camera's up direction
	int fov;		// in degrees
} t_camera;

typedef struct s_scene
{
	t_camera global_cam;
	// pointeur vers un array d'objets a dessiner
	t_object *objects;
	int		number_of_obj;
	// liste des lumieres

	// lumiere ambiante

} t_scene;

void camera_pitch(t_camera *cam, float angle);
void camera_yaw(t_camera *cam, float angle);
void camera_roll(t_camera *cam, float angle);

#endif
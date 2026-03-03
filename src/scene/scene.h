#ifndef SCENE_H
#define SCENE_H

#include "../math/math.h"
#include <stdbool.h>

typedef struct s_mlx_img
{
	void *img_ptr;
	int width;
	int height;
	char *first_pixel;
	int size_line;
	int bits_per_pixel;
	int endian;

} t_mlx_img;

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
	float radius;
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
	t_sphere sphere;
	t_plane plane;
	t_cylinder cylinder;
} t_any_obj;

typedef struct s_object
{
	t_obj_type type;
	t_any_obj data;
	t_vec3 color;
	bool checker;
	t_mlx_img texture;
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
	int number_of_obj;
	// liste des lumieres
	t_light *lights;
	int number_of_light;
	// lumiere ambiente
	t_light ambient_light;


} t_scene;

void camera_pitch(t_camera *cam, float angle);
void camera_yaw(t_camera *cam, float angle);
void camera_roll(t_camera *cam, float angle);

t_vec3 camera_pixel_to_vector(t_camera camera, int pixel_x, int pixel_y);

#endif
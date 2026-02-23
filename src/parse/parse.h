#ifndef PARSE_H
#define PARSE_H

#include "../math/math.h"

char *get_next_line_strip_nl(int fd);
int read_float(const char **s, float *out);
int read_vec3(const char *s, t_vec3 *out);
int read_rgb(const char *s, t_vec3 *out);

int parse_scene_file(t_scene *scene, const char *filename);

#endif
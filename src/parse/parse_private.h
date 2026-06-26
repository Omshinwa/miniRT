/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_private.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 11:15:18 by wiwu              #+#    #+#             */
/*   Updated: 2026/03/09 11:15:19 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_PRIVATE_H
#define PARSE_PRIVATE_H

#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>

#include "../app/app.h"
#include "../math/math.h"
#include "../raytrace/scene_object.h"

typedef enum e_token_type {
    T_INVALID,
    T_VEC,
    T_UNIT,
    T_FLOAT_UNIT,
    T_FLOAT,
    T_DIAMETER,
    T_FOV,
    T_RGB,
    T_RGB_OR_TEXTURE,
    T_IS_CHECKERBOARD,
    T_TEXTURE,
} t_token_type;

typedef struct s_field {
    const char *name;
    const t_token_type type;
    const bool required;
    const size_t offset;
} t_field;

typedef struct s_instruction {
    const char *id;
    const t_obj_type type;
    const bool should_be_unique;
    size_t scene_target;
    const t_field fields[5];
} t_instruction;

typedef struct s_parse_args {
    t_obj_type type;
    char **tokens;
    const t_field *fields;
    int i;
    void *dest;
} t_parse_args;

bool print_err(
    const char *msg, const char *wrong_data, const bool should_keep_going);

char *get_next_line_strip_nl(int fd);

bool parse_line(char *line, t_app *app);

bool do_non_shape_instruction(t_app *app, t_parse_args *args);
bool do_shape_instruction(t_app *app, t_parse_args *args);

bool parse_fields(t_app *app, t_parse_args *args);

bool read_float(char **s, float *out);
bool read_rgb(char *s, t_vec3 *out);
bool read_vec3(char *s, t_vec3 *out);

int setup_scene(t_app *app, char *filename);

#endif

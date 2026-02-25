#ifndef PARSE_PRIVATE_H
#define PARSE_PRIVATE_H

#include "../math/math.h"
#include "../app/app.h"

char *get_next_line_strip_nl(int fd);

/* ── token types ─────────────────────────────────────────────────────────── */

typedef enum e_token_type
{
	T_VEC,		/* "x,y,z"  → t_vec3  (any range)           */
	T_UNIT,		/* "x,y,z"  → t_vec3  (components ∈ [-1,1]) */
	T_FLOAT,	/* "f"      → float                          */
	T_DIAMETER, /* "d"      → float stored as radius d/2     */
	T_FOV,		/* "n"      → int [0, 180]                   */
	T_RGB,		/* "r,g,b"  → t_vec3 scaled 0-1              */
	T_INVALID,
} t_token_type;

typedef struct s_field
{
	const char *name;
	const t_token_type type;
	const bool required;
	const size_t offset;
} t_field;

typedef struct s_instruction
{
	const char *id;
	const size_t scene_target;
	const t_field fields[5];
} t_instruction;

bool parse_line(char *line, t_scene *scene);
int parse_fields(int index, char **tokens, const t_field *fields, void *dest);
#endif
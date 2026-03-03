#ifndef MAIN_H
#define MAIN_H

// Those are macros that should be avaiable in the whole project
#define WINDOW_X 800
#define WINDOW_Y 600
#define WINDOW_RATIO (float)WINDOW_X / (float)WINDOW_Y

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h> // printf

#include "../libft/libft.h"
#include "../minilibx-linux/mlx.h"

#include "math/math.h"
#include "app/app.h"
#include "render/render.h"

#endif
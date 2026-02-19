#ifndef HOOKS_H
#define HOOKS_H

#include "../main.h"

int on_mouse_input(int keycode, int mouse_x, int mouse_y, t_app *app);
int on_no_input(t_app *app);
int on_key_input(int keycode, t_app *app);

#endif
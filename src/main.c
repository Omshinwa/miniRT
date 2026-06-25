/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 11:06:27 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/18 06:32:14 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static const int CROSS_BUTTON = 17;

// MLX hook wrapper: calls exit_n_clean on window close
static int hook_exit(t_app *app)
{
    exit_n_clean(app, NULL);
    return (0);
}

// Entry point: create app, parse scene file, render and start MLX event loop
int main(int argc, char **argv)
{
    t_app *app;

    app = create_app();
    if (!app)
        exit_n_clean(app, RED "Failed to create app." RESET);
    if (argc == 1) {
        printf(RED "[WARNING] No scene file given. Falling back to "
                   "`scenes/default.rt`.\n Syntax should be `./miniRT "
                   "<filepath>`." RESET);
        setup_scene(app, "scenes/default.rt");
    } else if (argc == 2)
        setup_scene(app, argv[1]);
    else
        exit_n_clean(
            NULL, RED "[ERROR] Format should be `./miniRT <filepath>`" RESET);
    mlx_hook(app->win, CROSS_BUTTON, 0, hook_exit, app);
    redraw(app);
    mlx_loop(app->mlx);
    return (0);
}

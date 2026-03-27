/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:15:51 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/12 15:16:52 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../app/app.h"

static int	on_no_input(t_app *app)
{
	redraw(app);
	return (0);
}

// Note: it seems like mlx_hook(...) with masks isnt MacOS compatible, prefer:
// mlx_key_hook(app->win, on_key_input, app); (but it wont catch key holds)
void	hook_everything(t_app *app)
{
	mlx_hook(app->win, 2, 1L, on_key_input, app);
	mlx_mouse_hook(app->win, on_mouse_input, app);
	mlx_loop_hook(app->mlx, on_no_input, app);
}

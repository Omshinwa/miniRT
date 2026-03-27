/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:16:05 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/12 15:12:47 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"

char	*type_to_str(t_obj_type type)
{
	if (type & OBJ_SPHERE)
		return ("SPHERE");
	if (type & OBJ_PLANE)
		return ("PLANE");
	if (type & OBJ_CYLINDER)
		return ("CYLINDER");
	if (type & OBJ_CONE)
		return ("CONE");
	if (type & OBJ_CAMERA)
		return ("CAMERA");
	return ("UNDEFINED");
}

// This works using a Linear Congruential Generator
// If you change 6 and 5017, you need to have them validate
// several conditions
void	redraw(t_app *app)
{
	render_frame_multithreaded(app);
	mlx_put_image_to_window(app->mlx, app->win, app->img.mlx_img, 0, 0);
	draw_info(app);
}

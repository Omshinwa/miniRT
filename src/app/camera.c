/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:15:40 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/12 14:44:12 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"

void	rotate_around_axis(t_vec3 *v, t_vec3 k, float angle)
{
	float	c;
	float	s;
	t_vec3	term1;
	t_vec3	term2;
	t_vec3	term3;

	k = vec3_normalize(k);
	c = cosf(angle);
	s = sinf(angle);
	term1 = vec3_sca(*v, c);
	term2 = vec3_sca(vec3_cross(k, *v), s);
	term3 = vec3_sca(k, vec3_dot(k, *v) * (1.0f - c));
	*v = vec3_add(vec3_add(term1, term2), term3);
}

int	update_time(t_app *app)
{
	struct timeval	tv;
	long			sec_diff;
	long			usec_diff;
	long			elapsed;

	gettimeofday(&tv, NULL);
	sec_diff = (tv.tv_sec - app->time.tv_sec) * 1000000L;
	usec_diff = (tv.tv_usec - app->time.tv_usec);
	elapsed = sec_diff + usec_diff;
	app->time = tv;
	app->fps = 1000000L / elapsed;
	return (app->fps);
}

void	camera_pitch(t_camera *cam, float angle)
{
	rotate_around_axis(&cam->forward, cam->right, angle);
	rotate_around_axis(&cam->up, cam->right, angle);
	cam->forward = vec3_normalize(cam->forward);
	cam->up = vec3_normalize(cam->up);
	cam->right = vec3_normalize(vec3_cross(cam->up, cam->forward));
}

void	camera_yaw(t_camera *cam, float angle)
{
	t_vec3	axis;

	axis = cam->up;
	rotate_around_axis(&cam->forward, axis, angle);
	rotate_around_axis(&cam->right, axis, angle);
	cam->forward = vec3_normalize(cam->forward);
	cam->right = vec3_normalize(cam->right);
	cam->up = vec3_normalize(vec3_cross(cam->forward, cam->right));
}

void	camera_roll(t_camera *cam, float angle)
{
	t_vec3	axis;

	axis = cam->forward;
	rotate_around_axis(&cam->right, axis, angle);
	rotate_around_axis(&cam->up, axis, angle);
	cam->right = vec3_normalize(cam->right);
	cam->up = vec3_normalize(cam->up);
	cam->forward = vec3_normalize(vec3_cross(cam->right, cam->up));
}

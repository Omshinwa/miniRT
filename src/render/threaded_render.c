/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threaded_render.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wiwu <wiwu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 10:10:00 by wiwu              #+#    #+#             */
/*   Updated: 2026/03/18 10:10:00 by wiwu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_private.h"
#include <pthread.h>

#define TILE_SIZE 32
#define MAX_RENDER_THREADS 64

typedef struct s_thread_job
{
	t_app	*app;
	int		thread_id;
	int		thread_count;
	int		tile_count_x;
	int		tile_count_y;
	int		render_pass;
	int		local_pos_x;
	int		local_pos_y;
}t_thread_job;

static int	get_thread_count(void)
{
	long	cpu_count;

	cpu_count = sysconf(_SC_NPROCESSORS_ONLN);
	if (cpu_count <= 1)
		return (1);
	if (cpu_count > MAX_RENDER_THREADS)
		cpu_count = MAX_RENDER_THREADS;
	return ((int)cpu_count);
}

static void	set_image_pixel_at(t_app *app, int x, int y, int color)
{
	int	*buffer;
	int	bytes_per_pixel;

	bytes_per_pixel = app->img.bits_per_pixel / 8;
	buffer = (int *)app->img.first_pixel;
	buffer[(y * app->img.size_line / bytes_per_pixel) + x] = color;
}

static void	render_tile(t_thread_job *job, int tile_x, int tile_y)
{
	int	x_start;
	int	y_start;
	int	x_end;
	int	y_end;
	int	pos[2];

	x_start = tile_x * TILE_SIZE;
	y_start = tile_y * TILE_SIZE;
	x_end = x_start + TILE_SIZE;
	if (x_end > job->app->img.width)
		x_end = job->app->img.width;
	y_end = y_start + TILE_SIZE;
	if (y_end > job->app->img.height)
		y_end = job->app->img.height;
	pos[0] = x_start - 1;
	while (++pos[0] < x_end)
	{
		pos[1] = y_start - 1;
		while (++pos[1] < y_end)
		{
			if (pos[0] % job->render_pass == job->local_pos_x
				&& pos[1] % job->render_pass == job->local_pos_y)
				set_image_pixel_at(job->app, pos[0], pos[1],
					calc_pixel_color(job->app, pos[0], pos[1]));
		}
	}
}

static void	*render_thread(void *arg)
{
	t_thread_job	*job;
	int				thread_tile;
	int				tile_count;

	job = (t_thread_job *)arg;
	tile_count = job->tile_count_x * job->tile_count_y;
	thread_tile = job->thread_id;
	while (thread_tile < tile_count)
	{
		render_tile(job, thread_tile % job->tile_count_x,
			thread_tile / job->tile_count_x);
		thread_tile += job->thread_count;
	}
	return (NULL);
}

void	render_frame_multithreaded(t_app *app)
{
	int				thread_count;
	int				i;
	int				offset;
	int				local_pos_x;
	int				local_pos_y;
	pthread_t		threads[MAX_RENDER_THREADS];
	t_thread_job	jobs[MAX_RENDER_THREADS];
	bool			thread_started[MAX_RENDER_THREADS];
	static int		frame;
	const int		render_pass = 6;

	thread_count = get_thread_count();
	offset = render_pass * render_pass;
	local_pos_x = frame % render_pass;
	local_pos_y = frame / render_pass;
	jobs[0].tile_count_x = (app->img.width + TILE_SIZE - 1) / TILE_SIZE;
	jobs[0].tile_count_y = (app->img.height + TILE_SIZE - 1) / TILE_SIZE;
	i = -1;
	while (++i < thread_count)
	{
		jobs[i] = (t_thread_job){app, i, thread_count,
			jobs[0].tile_count_x, jobs[0].tile_count_y,
			render_pass, local_pos_x, local_pos_y};
		thread_started[i] = false;
		if (pthread_create(&threads[i], NULL, render_thread, &jobs[i]) != 0)
			render_thread(&jobs[i]);
		else
			thread_started[i] = true;
	}
	i = -1;
	while (++i < thread_count)
	{
		if (thread_started[i])
			pthread_join(threads[i], NULL);
	}
	frame = (frame * 5017 + 1) % offset;
}

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
#define RENDER_PASS 6

typedef struct s_thread_pool
{
	pthread_t			threads[MAX_RENDER_THREADS];
	pthread_barrier_t	start_barrier;
	pthread_barrier_t	end_barrier;
	int					thread_count;
	bool				shutdown;
	t_app				*app;
	int					tile_count_x;
	int					tile_count_y;
	int					render_pass;
	int					local_pos_x;
	int					local_pos_y;
}	t_thread_pool;

static t_thread_pool	g_pool;
static int				g_thread_ids[MAX_RENDER_THREADS];

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

static void	render_tile(int tile_x, int tile_y)
{
	int	x_start;
	int	y_start;
	int	x_end;
	int	y_end;
	int	pos[2];

	x_start = tile_x * TILE_SIZE;
	y_start = tile_y * TILE_SIZE;
	x_end = x_start + TILE_SIZE;
	if (x_end > g_pool.app->img.width)
		x_end = g_pool.app->img.width;
	y_end = y_start + TILE_SIZE;
	if (y_end > g_pool.app->img.height)
		y_end = g_pool.app->img.height;
	pos[0] = x_start - 1;
	while (++pos[0] < x_end)
	{
		pos[1] = y_start - 1;
		while (++pos[1] < y_end)
		{
			if (pos[0] % g_pool.render_pass == g_pool.local_pos_x
				&& pos[1] % g_pool.render_pass == g_pool.local_pos_y)
				set_image_pixel_at(g_pool.app, pos[0], pos[1],
					calc_pixel_color(g_pool.app, pos[0], pos[1]));
		}
	}
}

static void	*render_thread(void *arg)
{
	int	thread_id;
	int	tile_count;
	int	thread_tile;

	thread_id = *(int *)arg;
	while (1)
	{
		pthread_barrier_wait(&g_pool.start_barrier);
		if (g_pool.shutdown)
			break ;
		tile_count = g_pool.tile_count_x * g_pool.tile_count_y;
		thread_tile = thread_id;
		while (thread_tile < tile_count)
		{
			render_tile(thread_tile % g_pool.tile_count_x,
				thread_tile / g_pool.tile_count_x);
			thread_tile += g_pool.thread_count;
		}
		pthread_barrier_wait(&g_pool.end_barrier);
	}
	return (NULL);
}

static void	init_thread_pool(t_app *app)
{
	int	i;

	g_pool.thread_count = get_thread_count();
	g_pool.app = app;
	g_pool.shutdown = false;
	pthread_barrier_init(&g_pool.start_barrier, NULL,
		g_pool.thread_count + 1);
	pthread_barrier_init(&g_pool.end_barrier, NULL,
		g_pool.thread_count + 1);
	i = -1;
	while (++i < g_pool.thread_count)
	{
		g_thread_ids[i] = i;
		pthread_create(&g_pool.threads[i], NULL, render_thread,
			&g_thread_ids[i]);
	}
}

void	destroy_thread_pool(void)
{
	int	i;

	if (g_pool.thread_count == 0)
		return ;
	g_pool.shutdown = true;
	pthread_barrier_wait(&g_pool.start_barrier);
	i = -1;
	while (++i < g_pool.thread_count)
		pthread_join(g_pool.threads[i], NULL);
	pthread_barrier_destroy(&g_pool.start_barrier);
	pthread_barrier_destroy(&g_pool.end_barrier);
	g_pool.thread_count = 0;
}

void	render_frame_multithreaded(t_app *app)
{
	static int		frame;
	static bool		initialized;
	const int		render_pass = RENDER_PASS;

	if (!initialized)
	{
		init_thread_pool(app);
		initialized = true;
	}
	g_pool.app = app;
	g_pool.render_pass = render_pass;
	g_pool.local_pos_x = frame % render_pass;
	g_pool.local_pos_y = frame / render_pass;
	g_pool.tile_count_x = (app->img.width + TILE_SIZE - 1) / TILE_SIZE;
	g_pool.tile_count_y = (app->img.height + TILE_SIZE - 1) / TILE_SIZE;
	pthread_barrier_wait(&g_pool.start_barrier);
	pthread_barrier_wait(&g_pool.end_barrier);
	frame = (frame * 5017 + 1) % (render_pass * render_pass);
}

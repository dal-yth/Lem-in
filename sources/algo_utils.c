/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 14:40:30 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/27 22:35:42 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Inits queue
** Mallocs and assigns all values to -1
** Sets the first room in the queue to be starting room.
*/

void		init_queue(t_lem *core)
{
	int i;

	i = 0;
	if (!(LINK->queue = (int*)malloc(sizeof(int) * DATA->rooms + 1)))
		ft_error("Malloc error");
	while (i < (DATA->rooms + 1))
	{
		LINK->queue[i] = -1;
		i += 1;
	}
	LINK->queue[0] = 0;
	ROOM[0]->queue = 1;
}

/*
** Checks bfs queue
** Makes sure there is no flow going to the vertex and
** the vertex is not queued already.
*/

int			check_queue(t_lem *core, int from, int to, int user)
{
	if (user == 1)
	{
		if (LINK->dinic[from][to] == 1 || ROOM[LINK->adj[from][to]]->queue == 1)
			return (0);
	}
	else if (user == 2)
	{
		if (LINK->dinic[from][to] <= 0 ||
			LINK->visited[LINK->adj[from][to]] == 1 ||
			ROOM[LINK->adj[from][to]]->queue == 1)
			return (0);
	}
	return (1);
}

/*
** Resets levels and queue for next bfs
*/

void		reset_bfs(t_lem *core)
{
	int	i;

	i = 0;
	while (i < DATA->rooms)
	{
		ROOM[i]->level = 0;
		ROOM[i]->queue = 0;
		LINK->queue[i] = -1;
		i += 1;
	}
	LINK->queue[i] = -1;
	LINK->queue[0] = 0;
	ROOM[0]->queue = 1;
}

/*
** Creates and allocates dinics array
** Dinics array is copy of adjacency list but it has flow data
** instead of room data
*/

void		create_dinics(t_lem *core)
{
	int	y;
	int	x;

	y = 0;
	x = 1;
	if (!(LINK->dinic = (int**)malloc(sizeof(int*) * DATA->rooms)))
		ft_error("Malloc error");
	while (y < DATA->rooms)
	{
		if (!(LINK->dinic[y] = (int*)malloc(sizeof(int) * DATA->rooms + 1)))
			ft_error("Malloc error");
		LINK->dinic[y][0] = LINK->adj[y][0];
		while (LINK->adj[y][x] != -1)
		{
			LINK->dinic[y][x] = 0;
			x += 1;
		}
		LINK->dinic[y][x] = -1;
		x = 1;
		y += 1;
	}
}

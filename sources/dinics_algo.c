/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinics_algo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 15:04:34 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/28 07:56:02 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Breath first search.
** Assigns start room to queue and attempts to find adjacents
** of each room in queue. Assigns levels to rooms as they are found.
** Check queue makes sure same room is not recorded twice and rooms
** with existing dinics flow are skipped.
*/

int			bfs(t_lem *core, int user, int i)
{
	int x;
	int y;

	y = 0;
	x = 1;
	ROOM[0]->level = 0;
	(user == 2) ? (LINK->visited[0] = 1) : 0;
	while (LINK->queue[y] != -1)
	{
		while (LINK->adj[LINK->queue[y]][x] != -1)
		{
			if (check_queue(core, LINK->queue[y], x, user))
			{
				LINK->queue[i] = LINK->adj[LINK->queue[y]][x];
				ROOM[LINK->adj[LINK->queue[y]][x]]->level =
				ROOM[LINK->queue[y]]->level + 1;
				ROOM[LINK->adj[LINK->queue[y]][x]]->queue = 1;
				i += 1;
			}
			x += 1;
		}
		x = 1;
		y += 1;
	}
	return (ROOM[LASTROOM]->level);
}

/*
** Verifies the path from node y to the end.
** Uses dfs method of moving deeper along leveled path.
*/

int			verify_path(t_lem *core, int y)
{
	int x;

	x = 1;
	if (y == (LASTROOM))
		return (1);
	while (LINK->adj[y][x] != -1)
	{
		if (LINK->dinic[y][x] <= 0 &&
			ROOM[y]->level + 1 == ROOM[LINK->adj[y][x]]->level)
		{
			if (verify_path(core, LINK->adj[y][x]))
				return (1);
		}
		x += 1;
	}
	return (0);
}

/*
** Good_choice attempts to determine if dfs should chooce
** the current or upcoming node to traverse.
** Basic idea is that if the node matches all requirements:
** level +1, no dinics flow or negative flow. Then we need
** to check further because there may be more than one
** adjacent node that matches these conditions. And that
** adjacent node may be better for path diversity.
** For this choice we use room flow, whenever dfs traverses
** through a path that leads to end node, it gives rooms it
** flows through a flow value. This means the node has been used
** before but it does not stop another flow from going through it.
** However if there are two or more nodes that both match the
** conditions of level +1 and no/neg flow, the one without
** room flow is used. This does not change the maximum flow, but
** it changes the amount of rooms that are used to reach it.
*/

int			good_choice(t_lem *core, int y, int x, int z)
{
	if (ROOM[y]->level + 1 == ROOM[LINK->adj[y][x]]->level &&
		LINK->dinic[y][x] <= 0)
	{
		if (ROOM[LINK->adj[y][x]]->flow == 0 ||
			LINK->adj[y][x] == LASTROOM)
			return (1);
		while (LINK->adj[y][z] != -1)
		{
			if (ROOM[y]->level + 1 == ROOM[LINK->adj[y][z]]->level
				&& LINK->dinic[y][z] <= 0 && z != x)
				if (ROOM[LINK->adj[y][z]]->flow == 0)
					if (verify_path(core, LINK->adj[y][z]))
						return (0);
			z += 1;
		}
		return (1);
	}
	else
		return (0);
}

/*
** Depth first search
** Goes through the adjacency list using levels assigned by bfs.
** Always moves to one level higher from the current position.
** Recursion ends when the exit room is found and recursion crashes
** down and assigns flow to rooms on the path.
*/

int			dfs(t_lem *core, int y)
{
	int x;
	int z;

	x = 1;
	z = 1;
	if (y == (LASTROOM))
		return (1);
	while (LINK->adj[y][x] != -1)
	{
		if (good_choice(core, y, x, 1))
		{
			if (dfs(core, LINK->adj[y][x]))
			{
				ROOM[LINK->adj[y][x]]->flow = 1;
				LINK->dinic[y][x] += 1;
				while (LINK->adj[LINK->adj[y][x]][z] != y)
					z += 1;
				LINK->dinic[LINK->adj[y][x]][z] -= 1;
				z = 1;
				return (1);
			}
		}
		x += 1;
	}
	return (0);
}

/*
** Function that calls algorithms
** Inits queue, creates dinics array and loops bfs and dfs.
*/

void		algo(t_lem *core)
{
	init_queue(core);
	create_dinics(core);
	DATA->flow = 0;
	while (bfs(core, 1, 1))
	{
		while (dfs(core, 0))
			DATA->flow += 1;
		reset_bfs(core);
	}
	if (!DATA->flow)
		ft_error("There are no paths from start to end!");
	FLAG->algo = timer(core->time, 3);
}

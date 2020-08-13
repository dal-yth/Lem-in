/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buckets.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 14:44:15 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/27 22:30:24 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** DFS function that gets one path and marks it visited.
** Once bfs does the leveling of dinic flow nodes,
** dfs goes upwards level by level to find shortest paths
** and calls save_path to save the found paths into bucket.
*/

int			bucket_dfs(t_lem *core, int y, int i)
{
	int x;

	x = 1;
	if (y == LASTROOM)
	{
		save_path(core, i);
		return (1);
	}
	while (LINK->adj[y][x] != -1)
	{
		if (LINK->dinic[y][x] == 1 && LINK->visited[LINK->adj[y][x]] == 0 && \
			ROOM[y]->level + 1 == ROOM[LINK->adj[y][x]]->level)
		{
			LINK->path[i] = LINK->adj[y][x];
			if (bucket_dfs(core, LINK->adj[y][x], i + 1))
				return (1);
		}
		x += 1;
	}
	return (0);
}

/*
** Bucket loop for start adjacent bfs.
** All_buckets is called for each start adjacent.
** Runs bfs/dfs pathfinding loop and gets all
** shortest paths for every level configuration.
** Organize paths sorts the paths in level order
** and pathmath determines how good in terms of
** turns taken, the bucket is.
*/

void		adjacent_buckets(t_lem *core)
{
	int i;

	i = 1;
	while (LINK->adj[0][i] != -1)
	{
		if (LINK->dinic[0][i] == 1)
		{
			LINK->queue[0] = LINK->adj[0][i];
			ROOM[LINK->adj[0][i]]->queue = 1;
			ROOM[LINK->adj[0][i]]->level = 1;
			while (bfs(core, 2, 1))
			{
				bucket_dfs(core, 0, 1);
				reset_bfs(core);
				LINK->len[LINK->bucket] = LINK->cnt;
			}
			organize_paths(LINK->all_paths[LINK->bucket], LINK->cnt);
			pathmath(core, LINK->all_paths[LINK->bucket], LINK->bucket, 1);
			LINK->bucket += 1;
			LINK->cnt = 0;
			empty_visited(core);
			reset_bfs(core);
		}
		i += 1;
	}
}

/*
** Gets the general level bucket.
** Nodes are leveled with bfs and dfs does
** the pathfinding using visited values.
** Paths are then organized in length order
** from shortest to longest and pathmath
** determines the value of the bucket.
*/

void		first_bucket(t_lem *core)
{
	reset_bfs(core);
	empty_visited(core);
	while (bfs(core, 2, 1))
	{
		bucket_dfs(core, 0, 1);
		LINK->len[LINK->bucket] = LINK->cnt;
		reset_bfs(core);
	}
	organize_paths(LINK->all_paths[LINK->bucket], LINK->cnt);
	pathmath(core, LINK->all_paths[LINK->bucket], LINK->bucket, 1);
	LINK->bucket += 1;
	LINK->cnt = 0;
	reset_bfs(core);
	empty_visited(core);
}

/*
** Counts how many rooms have input and/or output forks.
** This number is used to determine if we want to use bruteforce
** or not.
*/

int			show_input_output(t_lem *core, int y, int x)
{
	int cntout;
	int cntin;
	int fork;

	fork = 0;
	cntin = 0;
	cntout = 0;
	while (y < DATA->rooms)
	{
		while (LINK->adj[y][x] != -1)
		{
			if (LINK->dinic[y][x] == 1)
				cntout += 1;
			if (LINK->dinic[y][x] == -1)
				cntin += 1;
			x += 1;
		}
		if (cntout > 1 || cntin > 1)
			fork += 1;
		cntout = 0;
		cntin = 0;
		x = 1;
		y += 1;
	}
	return (fork);
}

/*
** Start of bucket functions.
** Calls all different pathfinders to
** hopefully find the best combination of
** paths for the map.
** General bfs/dfs is called first, then
** bfs/dfs is looped for each start adjacent.
*/

void		get_buckets(t_lem *core)
{
	int fork;

	first_bucket(core);
	adjacent_buckets(core);
	FLAG->routes = timer(core->time, 3);
	timer(core->time, 2);
	fork = show_input_output(core, 0, 1);
	if (fork < 200)
		brute(core);
	FLAG->brute = timer(core->time, 3);
}

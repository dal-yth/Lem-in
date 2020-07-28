/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_paths.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 15:09:08 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/28 07:58:01 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Empties the visited array for next bfs.
*/

void	empty_visited(t_lem *core)
{
	int i;

	i = 1;
	while (i < DATA->rooms)
	{
		LINK->visited[i] = 0;
		i += 1;
	}
}

/*
** Saves the path found by dfs.
** Allocates new path for the current bucket
** and saves path to it, also marks rooms in path
** as visited.
*/

void	save_path(t_lem *core, int i)
{
	int x;
	int z;

	x = 1;
	z = 1;
	if (!(LINK->all_paths[LINK->bucket][LINK->cnt] =
		(int*)malloc(sizeof(int) * i)))
		ft_error("Malloc error");
	LINK->all_paths[LINK->bucket][LINK->cnt][0] = i - 1;
	while (x < i)
	{
		if (LINK->path[z] != LASTROOM)
			LINK->visited[LINK->path[z]] = 1;
		LINK->all_paths[LINK->bucket][LINK->cnt][x] = LINK->path[z];
		x += 1;
		z += 1;
	}
	LINK->cnt += 1;
	LINK->total += 1;
}

/*
** Initializes and allocates paths.
** DATA->flow * 2 is NEW for the disq buckets! remove *2 to revert back
*/

void	init_paths(t_lem *core, int i)
{
	if (!(LINK->visited = (int*)malloc(sizeof(int) * DATA->rooms)))
		ft_error("Malloc error");
	if (!(LINK->path = (int*)malloc(sizeof(int) * DATA->rooms)))
		ft_error("Malloc error");
	if (!(LINK->all_paths = (int***)malloc(sizeof(int**) * (DATA->flow + 2))))
		ft_error("Malloc error");
	if (!(LINK->len = (int*)malloc(sizeof(int) * (DATA->flow + 2))))
		ft_error("Malloc error");
	if (!(LINK->moves = (double*)malloc(sizeof(double) * (DATA->flow + 2))))
		ft_error("Malloc error");
	if (!(LINK->amount = (int*)malloc(sizeof(int) * (DATA->flow + 2))))
		ft_error("Malloc error");
	while (i < (DATA->flow + 2))
	{
		if (!(LINK->all_paths[i] = (int**)malloc(sizeof(int*) * DATA->flow)))
			ft_error("Malloc error");
		LINK->len[i] = 0;
		LINK->moves[i] = INTMAX;
		LINK->amount[i] = INTMAX;
		i += 1;
	}
	LINK->path[0] = 0;
	LINK->cnt = 0;
	LINK->bucket = 0;
	LINK->total = 0;
}

/*
** Check if the start and end are connected.
*/

int		check_if_best(t_lem *core)
{
	int x;

	x = 1;
	while (LINK->adj[0][x] != -1)
	{
		if (LINK->adj[0][x] == LASTROOM)
		{
			LINK->moves[0] = 1;
			LINK->amount[0] = 1;
			LINK->bucket = 1;
			LINK->rowcnt = 1;
			DATA->estimate = 1;
			LINK->total = 1;
			DATA->best_bucket = 0;
			FLAG->routes = timer(core->time, 3);
			FLAG->brute = timer(core->time, 3);
			FLAG->ants = timer(core->time, 3);
			FLAG->total = timer(core->time, 4);
			return (1);	
		}
		x += 1;
	}
	return (0);
}

/*
** Path main function.
*/

int		get_paths(t_lem *core)
{
	init_paths(core, 0);
	allocate_brute(core);
	empty_visited(core);
	reset_bfs(core);
	if (check_if_best(core))
		return (1);
	get_buckets(core);
	return (0);
}

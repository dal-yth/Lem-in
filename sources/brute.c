/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brute.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 15:16:25 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/27 20:48:19 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Saves path from bruteforce.
*/

void	brute_save(t_lem *core, int i)
{
	int x;
	int z;

	x = 1;
	z = 1;
	if (!(BRUTE->brute[BRUTE->b_cnt] = (int*)malloc(sizeof(int) * i)))
		ft_error("Malloc error");
	BRUTE->brute[BRUTE->b_cnt][0] = i - 1;
	while (x < i)
	{
		BRUTE->brute[BRUTE->b_cnt][x] = LINK->path[z];
		x += 1;
		z += 1;
	}
	BRUTE->b_cnt += 1;
}

/*
** Bruteforcing dfs function that goes through
** all dinics flow paths.
*/

int		brute_dfs(t_lem *core, int y, int i)
{
	int x;
	
	x = 1;
	if (y == LASTROOM)
	{
		if (i > DATA->estimate)
			return (1);
		brute_save(core, i);
		return (1);
	}
	while (LINK->adj[y][x] != -1)
	{
		if (LINK->dinic[y][x] == 1 && LINK->visited[LINK->adj[y][x]] == 0)
		{
			LINK->path[i] = LINK->adj[y][x];
			LINK->visited[LINK->adj[y][x]] = 1;
			brute_dfs(core, LINK->adj[y][x], i + 1);
			LINK->visited[LINK->adj[y][x]] = 0;
		}
		x += 1;
	}
	return (0);
}

/*
** Function to allocate and initialize brute values.
*/

void	allocate_brute(t_lem *core)
{
	int 			i;

	i = 0;
	LINK->visited[0] = 1;
	LINK->path[0] = 0;
	if (!(BRUTE = (t_brute*)malloc(sizeof(t_brute))))
		ft_error("Malloc error");
	if (!(BRUTE->brute = (int**)malloc(sizeof(int*) * (DATA->flow * DATA->flow))))
		ft_error("Malloc error");
	if (!(BRUTE->chosen = (int*)malloc(sizeof(int) * (DATA->flow + 1))))
		ft_error("Malloc error");
	if (!(BRUTE->best_choice = (int*)malloc(sizeof(int) * (DATA->flow + 1))))
		ft_error("Malloc error");
	while (i < DATA->flow + 1)
	{
		BRUTE->best_choice[i] = -1;
		BRUTE->chosen[i] = -1;
		i += 1;
	}
	BRUTE->b_cnt = 0;
}

/*
** Saves the best values found by bruteforce.
** Appends them to be the last bucket of paths.
*/

void	save_brute_bucket(t_lem *core, int i, int x)
{
	while (BRUTE->best_choice[i] != -1)
	{
		if (!(LINK->all_paths[LINK->bucket][i] = (int*)malloc(sizeof(int) * (BRUTE->brute[BRUTE->best_choice[i]][0] + 1))))
			ft_error("Malloc error");
		while (BRUTE->brute[BRUTE->best_choice[i]][x] != LASTROOM)
		{
			LINK->all_paths[LINK->bucket][i][x] = BRUTE->brute[BRUTE->best_choice[i]][x];
			x += 1;
		}
		LINK->all_paths[LINK->bucket][i][x] = BRUTE->brute[BRUTE->best_choice[i]][x];
		LINK->cnt += 1;
		LINK->total += 1;
		x = 0;
		i += 1;
	}
	LINK->len[LINK->bucket] = LINK->cnt;
	LINK->moves[LINK->bucket] = (int)BRUTE->best;
	LINK->amount[LINK->bucket] = i;
	LINK->bucket += 1;
}

/*
** Bruteforce algorithm.
** Uses visited array to find every possible path
** where dinics algorithm has assigned flow.
** Then organizes those paths to length order and
** Runs "smart" bruteforcing that compares most paths
** against most other paths.
*/

void	brute(t_lem *core)
{
	int i;
	int best;

	i = 0;
	empty_visited(core);
	DATA->best_bucket = pick_bucket(core);
	DATA->estimate = (int)LINK->moves[DATA->best_bucket];
	best = DATA->estimate;
	brute_dfs(core, 0, 1);
	organize_paths(BRUTE->brute, BRUTE->b_cnt);
	BRUTE->best = LINK->moves[DATA->best_bucket];
	while (i < BRUTE->b_cnt)
	{
		if (BRUTE->brute[i][0] < best + 1)
			get_brute_combis(core, i, best + 1);
		i += 1;
		best = (int)BRUTE->best;
	}
	if (BRUTE->best_choice[0] != -1)
		save_brute_bucket(core, 0, 0);
}

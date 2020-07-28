/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 15:05:09 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/27 19:24:16 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Frees the adjacency lists and rooms.
*/

void	free_rooms_adj_dinic(t_lem *core)
{
	int i;

	i = 0;
	while (i < DATA->rooms)
	{
		free(LINK->adj[i]);
		free(LINK->dinic[i]);
		free(ROOM[i]);
		i += 1;
	}
	free(LINK->adj);
	free(LINK->dinic);
}

/*
** Frees the 3d pointer that contains all paths.
*/

void	free_all_paths(t_lem *core)
{
	int y;
	int x;

	y = 0;
	x = 0;
	while (y < (DATA->flow + 2))
	{
		while (x < LINK->len[y])
		{
			free(LINK->all_paths[y][x]);
			x += 1;
		}
		x = 0;
		free(LINK->all_paths[y]);
		y += 1;
	}
	free(LINK->len);
	free(LINK->all_paths);
}

/*
** Frees brute paths array.
*/

void	free_brute(t_lem *core)
{
	int i;

	i = 0;
	while (i < DATA->flow)
	{
		free(BRUTE->brute[i]);
		i += 1;
	}
}

/*
** Frees the structs and individual arrays.
*/

void	free_structs(t_lem *core)
{
	free(DATA->final);
	free(DATA->input);
	free(LINK->queue);
	free(LINK->visited);
	free(LINK->path);
	free(LINK->moves);
	free(LINK->amount);
	free(LINK->antprint);
	free(BRUTE->chosen);
	free(BRUTE->best_choice);
	free_rooms_adj_dinic(core);
	free_all_paths(core);
	free_brute(core);
	free(ROOM);
	free(DATA);
	free(FLAG);
	free(LINK);
	free(BRUTE);
	free(core->time);
	free(core);
}

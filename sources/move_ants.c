/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_ants.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 15:18:19 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/27 22:31:41 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Checks if there are still ants present somewhere
** along the chosen paths. Starts with 0 room because
** all ants are packed in there in the start.
*/

int		ants_in_hill(t_lem *core, int bucket)
{
	int y;
	int x;

	y = 0;
	x = 1;
	if (ROOM[0]->ant <= DATA->ants)
		return (1);
	while (y < LINK->amount[bucket])
	{
		while (LINK->all_paths[bucket][y][x] != LASTROOM)
		{
			if (ROOM[LINK->all_paths[bucket][y][x]]->ant)
				return (1);
			x += 1;
		}
		x = 1;
		y += 1;
	}
	return (0);
}

/*
** The function that actually moves the ant.
** Deletes ant id from previous room, except if it is start.
** With start, the value is incremented since we use the value
** as id. So when we check the start ant, we know which ant
** is about to leave next.
*/

void	move_ant(t_lem *core, int from, int to)
{
	ROOM[to]->ant = ROOM[from]->ant;
	if (from)
		ROOM[from]->ant = 0;
	else
		ROOM[from]->ant += 1;
	save_to_antprint(core, ROOM[to]->ant, ROOM[to]->name);
}

/*
** Deals with and movement from start to first adjacent.
*/

int		do_first_nodes(t_lem *core, int bucket, int *lens)
{
	int y;

	y = 0;
	if (ROOM[0]->ant > DATA->ants)
		return (0);
	while (y < LINK->amount[bucket])
	{
		if (lens[y] != 0)
		{
			move_ant(core, 0, LINK->all_paths[bucket][y][1]);
			lens[y] -= 1;
		}
		y += 1;
	}
	return (1);
}

/*
** Goes through the path and moves each ant on it.
** Starts from the end and moves up to the adjacent of start.
*/

void	do_path(t_lem *core, int bucket, int *lens)
{
	int y;
	int x;

	y = 0;
	while (y < LINK->amount[bucket])
	{
		x = LINK->all_paths[bucket][y][0];
		while (x > 1)
		{
			if (ROOM[LINK->all_paths[bucket][y][x - 1]]->ant)
				move_ant(core, LINK->all_paths[bucket][y][x - 1],
				LINK->all_paths[bucket][y][x]);
			x -= 1;
		}
		y += 1;
	}
	do_first_nodes(core, bucket, lens);
	save_to_antprint(core, 0, "\n");
	LINK->rowcnt += 1;
}

/*
** Move ants
*/

int		move_ants(t_lem *core)
{
	int bucket;
	int *lens;

	if (DATA->best)
		return (1);
	DATA->best_bucket = 0;
	ROOM[0]->ant = 1;
	LINK->rowcnt = 0;
	LINK->pos = 0;
	bucket = pick_bucket(core);
	DATA->best_bucket = bucket;
	DATA->estimate = (int)LINK->moves[bucket];
	LINK->mem = (DATA->ants * DATA->estimate * (3 + 2 + 2)
	+ (DATA->estimate * LINK->amount[bucket]));
	if (!(LINK->antprint = (char *)malloc(sizeof(char) * LINK->mem)))
		ft_error("Malloc error");
	lens = get_lens(core, bucket, 0);
	while (ants_in_hill(core, bucket))
		do_path(core, bucket, lens);
	LINK->antprint[LINK->pos] = '\0';
	FLAG->ants = timer(core->time, 3);
	FLAG->total = timer(core->time, 4);
	free(lens);
	return (0);
}

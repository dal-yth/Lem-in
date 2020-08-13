/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 15:38:57 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/27 17:43:03 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Pathmath attempts to calculate how many turns it takes
** to send all the ants through some buckets paths.
** Since paths are in length order from shortest to longest,
** we can keep checking only until adding the path to used ones
** does no longer shorten the turncount.
** Paths length is always the potential worst case scenario,
** it takes at least path len amount of turns to send 1 ant and
** that is the minimum requirement to be useful for us.
** New is what happens if the path is added to the pool of good
** paths the turns are counted, if both that and worst case of
** that path are better or equal to the best one found, the path
** is ok to travel.
*/

int		pathmath(t_lem *core, int **bucket, int z, int amount)
{
	int i;

	i = 1;
	DATA->cur_best = bucket[0][0] + DATA->ants - 1;
	DATA->total = bucket[0][0];
	while (i < LINK->len[z])
	{
		DATA->worst = bucket[i][0];
		DATA->new = (((bucket[i][0] + DATA->total) / (amount + 1)) + \
		(DATA->ants / (amount + 1)));
		if (DATA->worst <= DATA->cur_best && DATA->new <= DATA->cur_best)
		{
			DATA->total += bucket[i][0];
			DATA->cur_best = DATA->new;
		}
		else
			break ;
		amount += 1;
		i += 1;
	}
	LINK->moves[z] = DATA->cur_best;
	LINK->amount[z] = amount;
	return (amount);
}

/*
** Makes sure paths are organized from smallest to largest.
*/

void	organize_paths(int **bucket, int len)
{
	int *temp;
	int y;
	int x;

	y = 0;
	x = 1;
	while (y < len)
	{
		while (x < len)
		{
			if (bucket[y][0] > bucket[x][0])
			{
				temp = bucket[y];
				bucket[y] = bucket[x];
				bucket[x] = temp;
			}
			x += 1;
		}
		y += 1;
		x = y + 1;
	}
}

/*
** Finds the currently shortest path on lens array.
** The shortest path represents the one where we will
** send the next ant.
*/

int		get_shortest(t_lem *core, int bucket, int *lens)
{
	int y;
	int small;
	int pos;

	y = 0;
	small = lens[0];
	pos = 0;
	while (y < LINK->amount[bucket])
	{
		if (lens[y] < small)
		{
			small = lens[y];
			pos = y;
		}
		y += 1;
	}
	lens[pos] += 1;
	return (pos);
}

/*
** Creates an array where lengths of chosen buckets
** paths are stored.
*/

int		*get_lens(t_lem *core, int bucket, int y)
{
	int *temp;
	int *lens;
	int ant;

	ant = DATA->ants;
	y = 0;
	if (!(temp = (int*)malloc(sizeof(int) * LINK->amount[bucket])))
		ft_error("Malloc error");
	if (!(lens = (int*)malloc(sizeof(int) * LINK->amount[bucket])))
		ft_error("Malloc error");
	while (y < LINK->amount[bucket])
	{
		temp[y] = 0;
		lens[y] = LINK->all_paths[bucket][y][0];
		y += 1;
	}
	y = 0;
	while (ant)
	{
		y = get_shortest(core, bucket, lens);
		temp[y] += 1;
		ant -= 1;
	}
	free(lens);
	return (temp);
}

/*
** If there are multiple buckets, pick bucket checks
** which bucket has least amount of estimated turns.
*/

int		pick_bucket(t_lem *core)
{
	int		i;
	int		pos;
	double	small;

	i = 0;
	small = LINK->moves[0];
	pos = 0;
	while (i < LINK->bucket)
	{
		if (LINK->moves[i] && LINK->moves[i] < small)
		{
			small = LINK->moves[i];
			pos = i;
		}
		i += 1;
	}
	return (pos);
}

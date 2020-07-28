/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brute_combis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 15:22:25 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/28 14:52:33 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Pathmath function for bruteforce.
** Same as in move ants, except doesn't use bucket.
*/

void	brute_pathmath(t_lem *core, int i)
{
	int		amount;
	double	c_best;
	double	worst;
	double	total;
	double	new;

	amount = 1;
	c_best = BRUTE->brute[BRUTE->chosen[0]][0] + DATA->ants - 1;
	total = BRUTE->brute[BRUTE->chosen[0]][0];
	while (BRUTE->chosen[i] != -1)
	{
		worst = BRUTE->brute[BRUTE->chosen[i]][0];
		new = ((worst + total) / (amount + 1)) + (DATA->ants / (amount + 1));
		if (worst <= c_best && new <= c_best)
		{
			total += worst;
			c_best = new;
		}
		else
			break ;
		amount += 1;
		i += 1;
	}
	save_chosen(core, c_best);
}

/*
** Organizes the chosen brute array.
*/

void	organize_brute(t_lem *core)
{
	int temp;
	int y;
	int x;

	y = 0;
	x = 1;
	while (BRUTE->chosen[y] != -1)
	{
		while (BRUTE->chosen[x] != -1)
		{
			if (BRUTE->brute[BRUTE->chosen[y]][0] >
				BRUTE->brute[BRUTE->chosen[x]][0])
			{
				temp = BRUTE->chosen[y];
				BRUTE->chosen[y] = BRUTE->chosen[x];
				BRUTE->chosen[x] = temp;
			}
			x += 1;
		}
		y += 1;
		x = y + 1;
	}
}

/*
** Inner loop of brute combinations.
** Matches recorded arrays against new possible
** arrays by checking if any of their rooms are
** already visited.
*/

int		brute_loop(t_lem *core, int x, int j, int best)
{
	while (j < BRUTE->b_cnt)
	{
		if (BRUTE->brute[j][0] > best)
			return (0);
		if (set_visited(core, j))
		{
			BRUTE->chosen[x] = j;
			x += 1;
		}
		j += 1;
	}
	return (1);
}

/*
** Matches all paths against all other paths
** to find out optimal combinations.
*/

void	get_brute_combis(t_lem *core, int z, int best)
{
	int i;
	int x;
	int j;

	j = z;
	i = 0;
	x = 1;
	while (i < BRUTE->b_cnt)
	{
		if (BRUTE->brute[i][0] > best)
			break ;
		BRUTE->chosen[0] = i;
		set_visited(core, i);
		brute_loop(core, x, j, best);
		organize_brute(core);
		brute_pathmath(core, 1);
		empty_chosen(core);
		empty_visited(core);
		j = z;
		x = 1;
		i += 1;
	}
}

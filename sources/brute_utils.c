/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brute_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 15:22:25 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/28 19:52:33 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Empties the chosen paths string.
*/

void	empty_chosen(t_lem *core)
{
	int i;

	i = 0;
	while (i < DATA->flow + 1)
	{
		BRUTE->chosen[i] = -1;
		i += 1;
	}
}

/*
** Finds out if the path in question has visited rooms.
** If any room is visited, path is ignored, else all
** rooms are recorded and path is saved.
*/

int		set_visited(t_lem *core, int path_id)
{
	int i;

	i = 1;
	while (BRUTE->brute[path_id][i] != LASTROOM)
	{
		if (LINK->visited[BRUTE->brute[path_id][i]] == 1)
			return (0);
		i += 1;
	}
	i = 1;
	while (BRUTE->brute[path_id][i] != LASTROOM)
	{
		LINK->visited[BRUTE->brute[path_id][i]] = 1;
		i += 1;
	}
	return (1);
}

/*
** Helper func that saves current best
** path to be the final best path.
*/

void	save_chosen(t_lem *core, double c_best)
{
	int i;

	i = 0;
	if (BRUTE->best > c_best)
	{
		BRUTE->best = c_best;
		while (BRUTE->chosen[i] != -1)
		{
			BRUTE->best_choice[i] = BRUTE->chosen[i];
			i += 1;
		}
		BRUTE->best_choice[i] = -1;
	}
}

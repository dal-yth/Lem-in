/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   antprint.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 14:41:28 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/28 08:02:03 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** For the special case of start-end path.
** If the best path (all ants can move in one turn)
** exists, do_best_moves prints the movements of each
** ant and deals with the last timers.
*/

void	do_best_moves(t_lem *core)
{
	int i;

	i = 0;
	while (i <= (int)DATA->ants)
	{
		ft_dprintf(FLAG->write_fd, "L%d-%s ", i, ROOM[LASTROOM]->name);
		i += 1;
	}
	ft_dprintf(FLAG->write_fd, "\n");
}

/*
** Calculates for ant mover how much space
** is required in the string. It is used to
** determine if we should malloc more.
*/

int		move_len(int ant, char *room)
{
	int i;
	int j;

	j = 0;
	if (ft_strequ("\n", room))
		return (1);
	else
	{
		i = ft_strlen(room);
		while (ant)
		{
			ant = ant / 10;
			j += 1;
		}
		return (i + j + 3);
	}
}

/*
** Writes correct ant/room/extras to antprint string.
** Necessary because printing ant movements individually
** takes way too much time compared to just recording them
** to one string and doing it with a single write.
*/

int		write_to_antprint(t_lem *core, int ant, char *room, int i)
{
	char *num;

	if (ft_strequ("\n", room))
	{
		LINK->antprint[LINK->pos++] = '\n';
		return (1);
	}
	LINK->antprint[LINK->pos++] = 'L';
	num = ft_itoa(ant);
	while (num[i])
	{
		LINK->antprint[LINK->pos++] = num[i];
		i += 1;
	}
	LINK->antprint[LINK->pos++] = '-';
	i = 0;
	while (room[i])
	{
		LINK->antprint[LINK->pos++] = room[i];
		i += 1;
	}
	LINK->antprint[LINK->pos++] = ' ';
	ft_strdel(&num);
	return (1);
}

/*
** Starting function for the save and write
** process of saving all ant movements to one
** string. Gets ant id and room where it is going.
** Gets len of ants movement and checks whether
** the antprint string has enough room to record
** the move. If not, temp string is allocated to
** be larger than the original, original is freec
** and set to be the temp string.
*/

void	save_to_antprint(t_lem *core, int ant, char *room)
{
	int		len;
	char	*temp;

	len = move_len(ant, room);
	if ((LINK->pos + len) < (int)LINK->mem)
		write_to_antprint(core, ant, room, 0);
	else
	{
		LINK->antprint[LINK->pos] = '\0';
		if (!(temp = (char*)malloc(sizeof(char) * LINK->pos + len + 1)))
			ft_error("Malloc error");
		LINK->mem = LINK->pos + len + 1;
		temp = ft_strcpy(temp, LINK->antprint);
		ft_strdel(&LINK->antprint);
		LINK->antprint = temp;
		write_to_antprint(core, ant, room, 0);
	}
}

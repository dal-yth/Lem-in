/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 14:53:55 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/27 23:30:34 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Checks for duplicate coordinates.
*/

void		check_coords(t_lem *core)
{
	int i;
	int j;

	i = 0;
	j = 1;
	while (i < DATA->rooms)
	{
		while (j < DATA->rooms)
		{
			if (ROOM[i]->x == ROOM[j]->x)
			{
				if (ROOM[i]->y == ROOM[j]->y)
					ft_error("Duplicate coordinates");
			}
			j += 1;
		}
		i += 1;
		j = i + 1;
	}
}

/*
** Extracts room name and coordinates.
** Works without malloc, since we only assign pointers to
** already allocated locations.
*/

int			extract_room(t_lem *core, int i, int cnt)
{
	ROOM[cnt]->name = DATA->input + i;
	while (DATA->input[i] != ' ')
		i += 1;
	DATA->input[i] = '\0';
	i += 1;
	ROOM[cnt]->x = ft_atoi(DATA->input + i);
	while (ft_isdigit(DATA->input[i]))
		i += 1;
	i += 1;
	ROOM[cnt]->y = ft_atoi(DATA->input + i);
	while (DATA->input[i] != '\n')
		i += 1;
	i += 1;
	return (i);
}

/*
** Finds the link position and compares it against
** the rooms to find out if such room exists.
*/

int			extract_dest(t_lem *core, int i)
{
	int cnt;
	int pos;

	cnt = 0;
	pos = i;
	while (DATA->input[pos] != '-')
		pos += 1;
	while (cnt < DATA->rooms)
	{
		if (!ft_strncmp(DATA->input + i, ROOM[cnt]->name, pos - i))
			return (cnt);
		cnt += 1;
	}
	return (-1);
}

/*
** Finds the link position and compares it against
** the rooms to find out if such room exists.
*/

int			extract_link(t_lem *core, int i)
{
	int cnt;
	int pos;

	cnt = 0;
	pos = i;
	while (DATA->input[pos] != '\n' && DATA->input[pos] != '\0')
		pos += 1;
	while (cnt < DATA->rooms)
	{
		if (!ft_strncmp(DATA->input + i, ROOM[cnt]->name, pos - i))
			return (cnt);
		cnt += 1;
	}
	return (-1);
}

/*
** Checks if duplicate link is found and assigns correct link value.
*/

void		do_link(t_lem *core, int dest, int link)
{
	int i;

	i = 0;
	while (i < DATA->rooms)
	{
		if (LINK->adj[dest][i] == link)
		{
			ft_dprintf(2, "link1 = %s, link2 = %s\n",
						ROOM[dest]->name, ROOM[link]->name);
			ft_error("Link error");
		}
		else if (LINK->adj[dest][i] == -1)
			break ;
		i += 1;
	}
	LINK->adj[dest][i] = link;
}

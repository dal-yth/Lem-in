/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 14:53:55 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/27 22:30:34 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Does the opposite links after initial links have been assigned
** The links need to be error checked for duplicates and if opposite
** links have already been assigned there might be fake errors.
*/

void		opposite_links(t_lem *core, int y, int x, int z)
{
	while (y < DATA->rooms)
	{
		while ((x < DATA->rooms) && (LINK->adj[y][x] != -1))
		{
			while (z < DATA->rooms)
			{
				if (LINK->adj[LINK->adj[y][x]][z] == y)
					break ;
				else if (LINK->adj[LINK->adj[y][x]][z] == -1)
				{
					LINK->adj[LINK->adj[y][x]][z] = y;
					break ;
				}
				z += 1;
			}
			z = 1;
			x += 1;
		}
		x = 1;
		y += 1;
	}
}

/*
** Saves links to our struct.
** Skips comments and extracts destinations and links.
** Destination is a node link is linking to.
** Append rooms to our struct.
*/

void		save_links(t_lem *core, int dest, int link, int i)
{
	i = DATA->linkpos;
	while (DATA->input[i])
	{
		if (DATA->input[i] == '#')
			i += skip_comment(core, i) + 1;
		else
		{
			dest = extract_dest(core, i);
			while (DATA->input[i] != '-')
				i += 1;
			link = extract_link(core, i + 1);
			while (DATA->input[i] != '\n' && DATA->input[i] != '\0')
				i += 1;
			if (DATA->input[i] == '\n')
				i += 1;
			if (dest < 0 || link < 0)
				ft_error("Wrong room name in link");
			else
				do_link(core, dest, link);
			dest = -1;
			link = -1;
		}
	}
}

/*
** Saves rooms to our struct.
** Goes through the part of the string with the rooms
** and calls extract room to assign a correct pointer
** and save coordinates of the room.
*/

void		save_rooms(t_lem *core)
{
	int i;
	int cnt;

	i = DATA->roompos;
	cnt = 1;
	while (i < DATA->linkpos)
	{
		if (DATA->input[i] == '#')
		{
			if (DATA->input[i + 1] == '#')
			{
				if (!ft_strncmp("##start\n", DATA->input + i, 8))
					i = extract_room(core, i + 8, 0);
				else if (!ft_strncmp("##end\n", DATA->input + i, 6))
					i = extract_room(core, i + 6, LASTROOM);
				else
					i += skip_comment(core, i) + 1;
			}
			else
				i += skip_comment(core, i) + 1;
		}
		else
		{
			i = extract_room(core, i, cnt);
			cnt += 1;
		}
	}
}

/*
** Inits rooms with starting values and allocates them.
** Allocates adjacency list arrays.
*/

void		fill_rooms(t_lem *core)
{
	int i;
	int j;

	j = 1;
	i = 0;
	while (i < DATA->rooms)
	{
		if (!(LINK->adj[i] = (int*)malloc(sizeof(int) * DATA->rooms + 1)))
			ft_error("Malloc error");
		LINK->adj[i][0] = i;
		while (j < (DATA->rooms + 1))
		{
			LINK->adj[i][j] = -1;
			j += 1;
		}
		j = 1;
		if (!(ROOM[i] = (t_room*)malloc(sizeof(t_room))))
			ft_error("Malloc error");
		ROOM[i]->ant = 0;
		ROOM[i]->level = 0;
		ROOM[i]->flow = 0;
		ROOM[i]->queue = 0;
		i += 1;
	}
}

/*
** Handles data structure creation and checking.
** Allocates data structures and calls functions
** to fill and check them.
*/

void		data_structs(t_lem *core)
{
	if (!(ROOM = (t_room**)malloc(sizeof(t_room*) * DATA->rooms)))
		ft_error("Malloc error");
	if (!(LINK = (t_link*)malloc(sizeof(t_link))))
		ft_error("Malloc error");
	if (!(LINK->adj = (int**)malloc(sizeof(int*) * DATA->rooms)))
		ft_error("Malloc error");
	fill_rooms(core);
	save_rooms(core);
	verify_rooms(core);
	check_coords(core);
	save_links(core, -1, -1, 0);
	opposite_links(core, 0, 1, 1);
	FLAG->data = timer(core->time, 3);
}

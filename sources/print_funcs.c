/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 15:25:49 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/28 19:05:39 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Prints every path from every bucket.
*/

int		print_all_paths(t_lem *core, int x, int i, int z)
{
	ft_dprintf(FLAG->write_fd, "%{RED}Bucket: %d%{EOC}\n", DATA->best_bucket);
	if (DATA->best)
	{
		ft_dprintf(FLAG->write_fd, "%{GREEN}Path %d: len = %d%{EOC}\n%s, ", \
					i, 1, ROOM[0]->name);
		ft_dprintf(FLAG->write_fd, "%s\n", ROOM[LASTROOM]->name);
		return (1);
	}
	while (z < LINK->amount[DATA->best_bucket])
	{
		ft_dprintf(FLAG->write_fd, "%{GREEN}Path %d: len = %d%{EOC}\n%s, ", \
			z + 1, LINK->all_paths[DATA->best_bucket][z][0], ROOM[0]->name);
		while (LINK->all_paths[DATA->best_bucket][z][x] != LASTROOM)
		{
			ft_dprintf(FLAG->write_fd, "%s, ", \
						ROOM[LINK->all_paths[DATA->best_bucket][z][x]]->name);
			x += 1;
		}
		ft_dprintf(FLAG->write_fd, "%s\n", \
					ROOM[LINK->all_paths[DATA->best_bucket][z][x]]->name);
		x = 1;
		z += 1;
	}
	return (1);
}

/*
** Prints out the format of data structures.
*/

void	show_structs(t_lem *core)
{
	int y;
	int x;

	y = 0;
	x = 1;
	ft_dprintf(FLAG->write_fd, "%{RED}DATA structs:%{EOC}\n");
	ft_dprintf(FLAG->write_fd, "Rooms are represented by id's from 0 to %d\n", \
				LASTROOM);
	ft_dprintf(FLAG->write_fd, "[ROOM id]:[(A)DJACENT, (F)LOW][...\n");
	while (y < DATA->rooms)
	{
		ft_dprintf(FLAG->write_fd, "[ROOM %2d]:", LINK->adj[y][0]);
		while (LINK->adj[y][x] != -1)
		{
			ft_dprintf(FLAG->write_fd, "[A%d,F%d]", LINK->adj[y][x], \
						LINK->dinic[y][x]);
			x += 1;
		}
		ft_dprintf(FLAG->write_fd, "\n");
		y += 1;
		x = 1;
	}
}

/*
** Prints debug information.
*/

void	print_debug_info(t_lem *core)
{
	ft_dprintf(FLAG->write_fd, "%{RED} --------------------%{EOC}\n");
	ft_dprintf(FLAG->write_fd, "%{RED}| Lem-in information |%{EOC}\n");
	ft_dprintf(FLAG->write_fd, "%{RED} --------------------%{EOC}\n");
	if (DATA->emptyline)
		ft_dprintf(FLAG->write_fd, "Map data was incomplete!\n");
	ft_dprintf(FLAG->write_fd, "Rooms: %d\n", DATA->rooms);
	ft_dprintf(FLAG->write_fd, "Links: %d\n", DATA->links);
	ft_dprintf(FLAG->write_fd, "Ants: %d\n", DATA->ants);
	ft_dprintf(FLAG->write_fd, "Buckets of paths: %d\n", LINK->bucket);
	ft_dprintf(FLAG->write_fd, "Total paths: %d\n", LINK->total);
	ft_dprintf(FLAG->write_fd, "Paths used: %d, from best bucket: %d\n",
				LINK->amount[DATA->best_bucket], DATA->best_bucket + 1);
	ft_dprintf(FLAG->write_fd, "%{RED}Times:%{EOC}\nInput reading: %fs\n", \
				FLAG->input);
	ft_dprintf(FLAG->write_fd, "Data processing: %fs\n", FLAG->data);
	ft_dprintf(FLAG->write_fd, "Dinic's algorithm: %fs\n", FLAG->algo);
	ft_dprintf(FLAG->write_fd, "Pathfinding: %fs\n", FLAG->routes);
	ft_dprintf(FLAG->write_fd, "Moving ants: %fs\n", FLAG->ants);
	ft_dprintf(FLAG->write_fd, "Bruteforcing: %fs\n", FLAG->brute);
	ft_dprintf(FLAG->write_fd, "Total time %f\n", FLAG->total);
	ft_dprintf(FLAG->write_fd, "Estimated turns: %d\n", DATA->estimate);
	ft_dprintf(FLAG->write_fd, "%{GREEN}Total turns: %d%{EOC}\n", LINK->rowcnt);
}

/*
** Opens and creates result.txt if needed.
*/

void	init_write_fd(t_lem *core)
{
	FLAG->write_fd = open("result.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (FLAG->write_fd == -1)
		ft_error("Could not open \"result.txt\"");
}

/*
** Prints core information.
*/

void	print_core(t_lem *core)
{
	if (FLAG->write_fd == 2)
		init_write_fd(core);
	if (!FLAG->minimal)
	{
		ft_dprintf(FLAG->write_fd, "%s\n\n", DATA->final);
		if (!FLAG->no_ants)
		{
			if (!DATA->best)
				ft_dprintf(FLAG->write_fd, "%s\n", LINK->antprint);
			else
				do_best_moves(core);
		}
	}
	if (FLAG->structs)
		show_structs(core);
	if (FLAG->debug)
		print_debug_info(core);
	if (FLAG->paths)
		print_all_paths(core, 1, 1, 0);
	if (FLAG->write_fd != 1)
		close(FLAG->write_fd);
}

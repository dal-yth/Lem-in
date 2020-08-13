/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 15:20:14 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/27 22:33:04 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Initializes several important values.
*/

void	init_core_values(t_lem *core)
{
	core->fd = 0;
	DATA->input = NULL;
	DATA->incomplete = NULL;
	DATA->rooms = 0;
	DATA->links = 0;
	DATA->linkpos = 0;
	DATA->roompos = 0;
	DATA->ants = 0;
	DATA->emptyline = 0;
	DATA->estimate = 0;
	DATA->best = 0;
	FLAG->debug = 0;
	FLAG->minimal = 0;
	FLAG->help = 0;
	FLAG->paths = 0;
	FLAG->ants = 0;
	FLAG->no_ants = 0;
	FLAG->structs = 0;
	FLAG->write_fd = 1;
}

/*
** Inits core.
** Allocates core and other structs.
*/

t_lem	*init_core(t_lem *core)
{
	if (!(core = (t_lem*)malloc(sizeof(t_lem))))
		ft_error("Malloc error");
	if (!(DATA = (t_data*)malloc(sizeof(t_data))))
		ft_error("Malloc error");
	if (!(FLAG = (t_flag*)malloc(sizeof(t_flag))))
		ft_error("Malloc error");
	if (!(core->time = (t_clock*)malloc(sizeof(t_clock))))
		ft_error("Malloc error");
	init_core_values(core);
	return (core);
}

/*
** Inits values before we move ants.
*/

void	init_move(t_lem *core)
{
	LINK->overdrive = 0;
	DATA->best_bucket = 0;
	ROOM[0]->ant = 1;
	LINK->rowcnt = 0;
	LINK->pos = 0;
}

/*
** Error function.
** Gets error message from user, displays Error: user message.
*/

void	ft_error(char *msg)
{
	if (msg)
		ft_dprintf(2, "ERROR: %s\n", msg);
	else
		ft_dprintf(2, "ERROR\n", msg);
	exit(0);
}

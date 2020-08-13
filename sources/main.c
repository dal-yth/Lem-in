/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 15:15:32 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/28 19:18:47 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Shows usage and possible options of lem_in.
*/

void	show_help(void)
{
	ft_printf("%{RED}Usage:%{EOC} ./lem-in < file.map ");
	ft_printf("%{RED}OR%{EOC} ./lem-in file.map\n");
	ft_printf("%{GREEN}Options:%{EOC}\n");
	ft_printf(" -h, --help\t\tDisplays help.\n");
	ft_printf(" -d, --debug\t\tDisplays debugging information.\n");
	ft_printf(" -p, --paths\t\tDisplays all used paths.\n");
	ft_printf(" -m, --minimal\t\tDisplays only other chosen options.\n");
	ft_printf(" -a, --no_ants\t\tDoes not display ant movements.\n");
	ft_printf(" -s, --data_structure\t\tDisplays data structure.\n");
	ft_printf(" -w, --write\t\tDisplays nothing, ");
	ft_printf("writes the result into \"result.txt\"\n");
	exit(0);
}

/*
** Parses through agrv arguments and attemps to find flags
** or file to read from.
*/

void	check_args(t_lem *core, int argc, char **argv, int i)
{
	while (i < argc)
	{
		if (ft_strequ(argv[i], "-h"))
			show_help();
		else if (ft_strequ(argv[i], "-d"))
			FLAG->debug = 1;
		else if (ft_strequ(argv[i], "-m"))
			FLAG->minimal = 1;
		else if (ft_strequ(argv[i], "-w"))
			FLAG->write_fd = 2;
		else if (ft_strequ(argv[i], "-p"))
			FLAG->paths = 1;
		else if (ft_strequ(argv[i], "-a"))
			FLAG->no_ants = 1;
		else if (ft_strequ(argv[i], "-s"))
			FLAG->structs = 1;
		else if (!core->fd)
		{
			core->fd = open(argv[i], O_RDONLY, 0777);
			(core->fd == -1) ? ft_error("Error! ./lem-in -h for help.") : 0;
		}
		else
			ft_error("Argument error! ./lem-in -h for help.");
		i += 1;
	}
}

/*
** Lem_in main.
** Initializes core values and starts the timer.
** Calls all other important functions:
** Check_args deals with flags and arguments to the program.
** Read_input reads input and does necessary error checking.
** Data_structs saves all rooms and links to proper structs.
** Algo does a modified dinic's algorithm to find max flow.
** Get_paths uses several methods to find best possible
** combination of paths (bucket) for the ants.
** Move ants does the actual moving and records it.
** Print_core prints out several things depending on options.
** Free_structs goes through all allocated stuff and frees it.
*/

int		main(int argc, char **argv)
{
	t_lem		*core;

	core = NULL;
	core = init_core(core);
	timer(core->time, 1);
	timer(core->time, 2);
	check_args(core, argc, argv, 1);
	read_input(core);
	timer(core->time, 2);
	data_structs(core);
	timer(core->time, 2);
	algo(core);
	timer(core->time, 2);
	if (get_paths(core))
		DATA->best = 1;
	timer(core->time, 2);
	move_ants(core);
	print_core(core);
	free_structs(core);
	return (0);
}

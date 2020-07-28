/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 15:11:41 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/27 22:06:26 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Checks for empty lines and finds linkpos.
** Lem-in must attempt to guide the ants even if
** all the information is not given.
*/

void			stop_check(t_lem *core, int i)
{
	while (DATA->input[i])
	{
		if (DATA->input[i] && DATA->input[i] == '#' &&
			DATA->input[i - 1] == '\n')
			i += skip_comment(core, i);
		if (DATA->input[i] == '\n' && DATA->input[i + 1] == '\n')
		{
			if (DATA->input[i + 2] != '\0')
			{
				DATA->emptyline = 1;
				DATA->incomplete = ft_strsub(DATA->input, 0, i);
				ft_strdel(&DATA->input);
				DATA->input = DATA->incomplete;
			}
			break ;
		}
		else if (!DATA->linkpos && DATA->input[i] == '-')
		{
			while (DATA->input[i] && DATA->input[i] != '\n')
				i -= 1;
			DATA->linkpos = i + 1;
		}
		i += 1;
	}
}

/*
** Gets the ants
** Finds first position where first character of the row is a digit.
** Uses atoi to extract the number and tests that there is nothing
** else on the row. Also checks if no ants were found at all or if
** number of ants was 0.
*/

int				check_ants(t_lem *core, int i)
{
	while (DATA->input[i])
	{
		if (DATA->input[i] == '#')
			i += skip_comment(core, i);
		else if (ft_isdigit(DATA->input[i]) && (i == 0 ||
				DATA->input[i - 1] == '\n'))
		{
			(DATA->input[i] == '0') ? ft_error("Ants error") : 0;
			DATA->ants = ft_atoi(DATA->input + i);
		}
		else
			ft_error("Wrong characters");
		if (DATA->ants)
		{
			while (ft_isdigit(DATA->input[i]))
				i += 1;
			(DATA->input[i] != '\n') ? ft_error("Ants error") : 0;
			break ;
		}
		i += 1;
	}
	(!DATA->ants) ? ft_error("Ants error") : 0;
	return (i + 1);
}

/*
** Finds the start and end room.
** Attempts to find # as the first character and checks if the
** line has ##start or ##end and skips the line.
** Calls room_format to check that the following lines actually
** have proper rooms.
*/

void			check_specials(t_lem *core, int i)
{
	int	start;
	int	end;

	start = 0;
	end = 0;
	while (DATA->input[i] && DATA->input[i] != '-')
	{
		if (DATA->input[i] == '#' && DATA->input[i - 1] == '\n')
		{
			if (!ft_strncmp(DATA->input + i, "##start\n", 8))
			{
				start += 1;
				room_format(core, i + 8);
			}
			else if (!ft_strncmp(DATA->input + i, "##end\n", 6))
			{
				end += 1;
				room_format(core, i + 6);
			}
			i += skip_comment(core, i);
		}
		i += 1;
	}
	(!start || !end) ? ft_error("Start or end missing") : 0;
}

/*
** Checks room and link formats.
** Starts from position where rooms start in input string.
** Skips comments and calls room_format to check each individual room.
** Second portion of the function does the same for links.
** No bad formatting or characters are allowed.
*/

void			check_rooms_links(t_lem *core)
{
	int	i;

	i = DATA->roompos;
	DATA->rooms -= 2;
	while (i < DATA->linkpos)
	{
		if (DATA->input[i] == '#' && DATA->input[i - 1] == '\n')
			i += skip_comment(core, i);
		else if (DATA->input[i] == '\n')
			i += 1;
		else
			i += room_format(core, i);
	}
	while (DATA->input[i])
	{
		if (DATA->input[i] == '#' && DATA->input[i - 1] == '\n')
			i += skip_comment(core, i);
		else if (DATA->input[i] == '\n')
			i += 1;
		else
			i += link_format(core, i);
	}
}

/*
** Reads from a file or standard input.
** Attempt to read from file first and if no file is found
** tries to read from standard input. Buffers are strjoined
** in the same manner as in get_next_line.
** Once reading is done, all checking functions are called.
*/

void			read_input(t_lem *core)
{
	char	buf[BUF_S + 1];
	char	*temp;
	int		i;

	while ((i = read(core->fd, buf, BUF_S)) > 0)
	{
		buf[i] = '\0';
		if (!DATA->input)
			DATA->input = ft_strnew(1);
		temp = ft_strjoin(DATA->input, buf);
		ft_strdel(&DATA->input);
		DATA->input = temp;
	}
	if (!DATA->input)
		ft_error("Empty file.");
	if (core->fd)
		close(core->fd);
	DATA->final = ft_strdup(DATA->input);
	stop_check(core, 0);
	if (!DATA->linkpos)
		ft_error("Not enough information");
	DATA->roompos = check_ants(core, 0);
	check_specials(core, DATA->roompos);
	check_rooms_links(core);
	FLAG->input = timer(core->time, 3);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 15:12:43 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/28 19:02:52 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

/*
** Checks room format.
** First checks the starting char, then loops through
** until it finds space, checks if actual space was found.
** Then checks for digits, space and digits again, making
** sure at the end that there is next line.
*/

int				room_format(t_lem *core, int i)
{
	int	j;

	j = i;
	if (DATA->input[i] == 'L')
		ft_error("Room starts with L");
	while (DATA->input[i] > 32 && DATA->input[i] < 127)
		i += 1;
	if (DATA->input[i] != ' ')
		ft_error("Wrong room format");
	i += 1;
	while (ft_isdigit(DATA->input[i]))
		i += 1;
	if (DATA->input[i] != ' ')
		ft_error("Wrong room format");
	i += 1;
	while (ft_isdigit(DATA->input[i]))
		i += 1;
	if (DATA->input[i] != '\n')
		ft_error("Wrong room format");
	DATA->rooms += 1;
	return (i - j);
}

/*
** Checks format of link.
** Checks starting character of links first.
** Then loops through chars until it finds '-'.
** Checks that the actual '-' is found and loops again
** for the second link with same rules.
*/

int				link_format(t_lem *core, int i)
{
	int	j;

	j = i;
	if (DATA->input[i] == 'L' || DATA->input[i] == ' ' || DATA->input[i] == '-')
		ft_error("Wrong room name in link");
	while (DATA->input[i] && DATA->input[i] != '-' &&
			DATA->input[i] > 32 && DATA->input[i] < 127)
		i += 1;
	if (DATA->input[i] != '-')
		ft_error("No link char found");
	i += 1;
	if (DATA->input[i] == 'L' || DATA->input[i] == ' ' ||
		DATA->input[i] == '-' || DATA->input[i] == '#' ||
		DATA->input[i] == '\n' || DATA->input[i] == '\0')
		ft_error("Wrong room name in link");
	while (DATA->input[i] && DATA->input[i] != '\n' &&
			DATA->input[i] > 32 && DATA->input[i] < 127)
		i += 1;
	DATA->links += 1;
	return (i - j);
}

/*
** Utility function that skips until it finds newline or \0.
** Returns the amount of chars skipped.
*/

int				skip_comment(t_lem *core, int i)
{
	int j;

	j = i;
	while (DATA->input[i] && DATA->input[i] != '\n')
		i += 1;
	return (i - j);
}

/*
** Check that no two rooms have the same name.
** Also checks that there arent too many ants.
*/

void			verify_rooms(t_lem *core)
{
	int i;
	int j;

	i = 0;
	if (DATA->ants > INTMAX)
		ft_error("Only up to INTMAX ants supported");
	while (i < DATA->rooms)
	{
		j = i + 1;
		while (j < DATA->rooms)
		{
			if (ft_strequ(ROOM[i]->name, ROOM[j]->name))
				ft_error("Duplicate room");
			j += 1;
		}
		i += 1;
	}
}

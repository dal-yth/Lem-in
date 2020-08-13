/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmakela <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/26 15:44:43 by jmakela           #+#    #+#             */
/*   Updated: 2020/07/28 18:55:51 by jmakela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H
# include "../libft/includes/libft.h"

/*
** Defines for readability.
*/

# define MALLOC_MAX		100000000000000
# define INTMAX			2147483647
# define LASTROOM		core->data->rooms -1
# define DATA			core->data
# define ROOM			core->room
# define LINK			core->link
# define DINIC			core->dinic
# define FLAG			core->flags
# define BRUTE			core->brute
# define BUF_S			2000000

/*
** Bruteforce structure
** Contains array for all bruteforce paths,
** Current chosen array and best array.
*/

typedef struct			s_brute
{
	int					**brute;
	int					*chosen;
	int					*best_choice;
	int					b_cnt;
	double				best;
}						t_brute;

/*
** Data struct, defined as DATA.
** Contains general information such as:
** Amount of rooms, links, ants...
** Also stores input, flow, path and ant info.
*/

typedef struct			s_data
{
	int					rooms;
	int					links;
	unsigned long int	ants;
	int					roompos;
	int					linkpos;
	int					flow;
	char				*input;
	char				*final;
	char				*incomplete;
	int					emptyline;
	int					estimate;
	int					best;
	int					best_bucket;
	double				new;
	double				worst;
	double				total;
	double				cur_best;
}						t_data;

/*
** Room struct, defined as ROOM
** ROOMs are stored as an array,
** numbered from 0 - LASTROOM.
** Each room has name, coordinates,
** ant (if any), flow and queue.
** And is the id of ant in room.
** Flow shows if the room has already
** been used during dinic's but is not
** an absolute value.
** Queue indicates if the room has been
** queued by the bfs.
*/

typedef struct			s_room
{
	char				*name;
	int					x;
	int					y;
	int					ant;
	int					level;
	int					flow;
	int					queue;
}						t_room;

/*
** Link struct, defined as LINK
** LINK holds all data structures relevant
** to solving our problem.
** Adj, holds the adjacency list of rooms.
** Dinic, holds adjacency list of flow between rooms.
** All_paths is bucket based 3d pointer and the way we
** use to keep track of path uniqueness.
** Path is the currently traversed path during dfs.
** Moves and amount hold estimates of how many turns
** the bucket takes and how many paths that bucket uses.
** Antprint is for printing all ant movements in one write.
*/

typedef struct			s_link
{
	int					*queue;
	int					**adj;
	int					**dinic;
	int					***all_paths;
	int					*len;
	int					*visited;
	int					*path;
	int					bucket;
	int					cnt;
	int					total;
	double				*moves;
	int					*amount;
	char				*antprint;
	unsigned long int	pos;
	unsigned long int	mem;
	int					rowcnt;
	int					overdrive;
}						t_link;

/*
** Flag structure, defined as FLAG
** FLAG holds values for potential choices
** the user can input for the program
** and keeps track of the time it takes
** to execute each important task.
** For more information about flags: ./lem_in -h
*/

typedef struct			s_flag
{
	int					debug;
	int					minimal;
	int					help;
	int					paths;
	int					no_ants;
	int					structs;
	int					write_fd;
	double				input;
	double				data;
	double				algo;
	double				routes;
	double				ants;
	double				total;
	double				brute;
}						t_flag;

/*
** Lem_in core structure.
** Holds pointers to all other structures
** and initial fd for input reading.
** Almost every function takes core as argument
** and this way we can keep the large amount of
** data required at hand at all times.
*/

typedef struct			s_lem
{
	t_data				*data;
	t_room				**room;
	t_link				*link;
	t_flag				*flags;
	t_brute				*brute;
	t_clock				*time;
	int					fd;
}						t_lem;

/*
** Reading and handling input
*/

void					read_input(t_lem *core);
int						link_format(t_lem *core, int i);
int						room_format(t_lem *core, int i);
int						skip_comment(t_lem *core, int i);

/*
** Error handling
*/

void					verify_rooms(t_lem *core);
void					check_coords(t_lem *core);

/*
** Data structures
*/

void					data_structs(t_lem *core);
int						extract_link(t_lem *core, int i);
int						extract_dest(t_lem *core, int i);
int						extract_room(t_lem *core, int i, int cnt);
void					do_link(t_lem *core, int dest, int link);

/*
** Dinics algorithm
*/

void					algo(t_lem *core);
int						bfs(t_lem *core, int user, int i);
int						dfs(t_lem *core, int y);
void					init_queue(t_lem *core);
int						check_queue(t_lem *core, int from, int to, int user);
void					reset_bfs(t_lem *core);
void					create_dinics(t_lem *core);

/*
** Path functions
*/

int						get_paths(t_lem *core);
int						move_ants(t_lem *core);
void					get_buckets(t_lem *core);
void					empty_visited(t_lem *core);
void					save_path(t_lem *core, int i);

/*
** Buckets functions
*/

int						bucket_dfs(t_lem *core, int y, int i);
int						pathmath(t_lem *core, int **bucket, int z, int amount);
void					organize_paths(int **bucket, int len);
void					do_best_moves(t_lem *core);

/*
** Bruteforce functions
*/

void					brute(t_lem *core);
void					get_brute_combis(t_lem *core, int z, int best);
void					save_chosen(t_lem *core, double c_best);
int						set_visited(t_lem *core, int path_id);
void					empty_chosen(t_lem *core);
void					allocate_brute(t_lem *core);

/*
** Ants moving functions
*/

void					save_to_antprint(t_lem *core, int ant,\
										char *room, int len);
int						pick_bucket(t_lem *core);
int						*get_lens(t_lem *core, int bucket, int y);
void					init_move(t_lem *core);

/*
** Utilities
*/

t_lem					*init_core(t_lem *core);
void					ft_error(char *msg);
void					free_structs(t_lem *core);

/*
** Print functions
*/

void					print_core(t_lem *core);
int						print_all_paths(t_lem *core, int x, int i, int z);
void					print_debug_info(t_lem *core);
void					show_structs(t_lem *core);

#endif

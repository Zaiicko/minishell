/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:14:40 by nicleena          #+#    #+#             */
/*   Updated: 2025/04/03 01:59:09 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <signal.h>
# include <sys/wait.h>

extern int	g_exit_status;

typedef enum s_node_type
{
	NODE_PIPE,
	NODE_COMMAND,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_AND,
	NODE_OR,
	NODE_APPEND,
	NODE_HEREDOC,
}	t_node_type;

typedef struct s_ast_node
{
	t_node_type				type;
	char					**args;
	char					*redir_file;
	struct s_ast_node		*r;
	struct s_ast_node		*l;
}	t_ast_node;

// typedef struct t_data;

// struct s_data
// {
//     int id;    
// };

void	readline_loop(void);
void	start_signals(void);
void	handle_sigint(int sig);
void	config_terminal(void);
t_ast_node	*new_node(t_node_type type);

#endif

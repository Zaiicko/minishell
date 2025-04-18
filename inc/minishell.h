/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:14:40 by nicleena          #+#    #+#             */
/*   Updated: 2025/04/18 02:08:34 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>

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

typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

// typedef struct t_data;

// struct s_data
// {
//     int id;    
// };

void		readline_loop(void);
void		start_signals(void);
void		handle_sigint(int sig);
void		load_history(void);
void		save_history(char *line);
void		exit_perror(char *msg);
t_ast_node	*new_node(t_node_type type);
t_ast_node	*new_command_node(char	**args);
t_ast_node	*new_pipe_node(t_ast_node *l_cmd, t_ast_node *r_cmd);
t_ast_node	*new_redir_node(t_node_type type, t_ast_node *cmd, char *target);
t_ast_node	*new_operator_node(t_node_type type, t_ast_node *l, t_ast_node *r);
t_token		*new_token(t_token_type type, char *value);
void		add_token_to_list(t_token **head, t_token *new_token);
void		free_token_list(t_token **head);
void		free_token(t_token *token);
t_token		*tokenize(char *input);
int			is_space(char c);
int			is_operator(char c);
int			handle_operator(char *input, int i, t_token **head);
int			handle_redirection(char *input, int i, t_token **head);
t_ast_node	*parse(t_token *tokens);
t_ast_node	*parse_logical(t_token **tokens);
t_ast_node	*parse_pipe(t_token **tokens);
t_ast_node	*parse_command(t_token **tokens);
int			count_command_args(t_token *tokens);
t_node_type	convert_type(t_token_type token_type);
t_ast_node	*handle_redirections(t_token **tokens, t_ast_node *cmd);
void		free_ast(t_ast_node *root);

#endif

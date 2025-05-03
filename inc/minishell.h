/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:14:40 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/03 18:34:42 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

extern int				g_exit_status;

typedef struct s_expander
{
	int					read_index;
	int					write_index;
	int					in_squotes;
	int					in_dquotes;
}						t_expander;

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*prev;
	struct s_env_var	*next;
}						t_env_var;

typedef struct s_env
{
	t_env_var			*head;
	t_env_var			*next;
}						t_env;

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
}						t_node_type;

typedef struct s_ast_node
{
	t_node_type			type;
	char				**args;
	char				*redir_file;
	struct s_ast_node	*r;
	struct s_ast_node	*l;
}						t_ast_node;

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
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}						t_token;

typedef struct s_data
{
	char				*input;
	t_token				*tokens;
	t_ast_node			*ast;
	t_env				*env;
	t_expander			exp;
}						t_data;

void					readline_loop(t_data *data);
void					process_user_input(t_data *data);
void					init_data(t_data *data, char **env);

void					start_signals(void);
void					handle_sigint(int sig);

void					load_history(void);
void					save_history(t_data *data);

void					exit_perror(char *msg);
void					free_all(t_data *data);
void					free_all_and_exit_perror(t_data *data, char *msg);

t_ast_node				*new_node(t_node_type type);
t_ast_node				*new_command_node(char **args);
t_ast_node				*new_pipe_node(t_ast_node *l_cmd, t_ast_node *r_cmd);
t_ast_node				*new_redir_node(t_node_type type, t_ast_node *cmd,
							char *target);
t_ast_node				*new_operator_node(t_node_type type, t_ast_node *l,
							t_ast_node *r);
void					free_ast(t_ast_node *root);

t_token					*new_token(t_token_type type, char *value);
void					add_token_to_list(t_token **head, t_token *new_token);
void					safe_add_token_to_list(t_data *data, t_token **head,
							t_token_type type, char *value);
void					free_token_list(t_token **head);
void					free_token(t_token *token);
t_token					*tokenize(t_data *data);

int						is_space(char c);
int						is_operator(char c);
int						handle_operator(t_data *data, int i, t_token **head);
int						handle_redirection(t_data *data, int i, t_token **head);

t_ast_node				*parse(t_data *data);
t_ast_node				*parse_logical(t_data *data, t_token **tokens);
t_ast_node				*parse_pipe(t_data *data, t_token **tokens);
t_ast_node				*parse_command(t_data *data, t_token **tokens);
int						count_command_args(t_token *tokens);
t_node_type				convert_type(t_token_type token_type);
t_ast_node				*handle_redirections(t_data *data, t_token **tokens,
							t_ast_node *cmd);
void					fill_command_args(t_data *data, t_token **tokens,
							char **args, int count);

t_env					*init_env(char **envp, t_data *data);
void					ft_setenv(t_env *env, char *key, char *value,
							t_data *data);
void					ft_unsetenv(t_env *env, char *key);
int						ft_unset(char **args, t_env *env);
int						execute_ast(t_ast_node *node, t_data *data);
int						exec_command(t_ast_node *node, t_data *data);
int						exec_pipe(t_ast_node *node, t_data *data);
int						is_builtin(char *cmd);
int						exec_builtin(char **args, t_env *env, t_data *data);
void					ft_tilde(t_env *env, t_data *data);
void					ft_cd_oldpwd(t_env *env, t_data *data);
int						ft_cd(char **args, t_env *env, t_data *data);
void					handle_cd(char **args);
void					ft_echo(char **arg);
void					ft_env(t_env *env);
void					ft_export(t_env *env);
void					ft_pwd(void);
void					ft_oldpwd(void);
int						handle_exit(char **args);
int						exec_pipe(t_ast_node *node, t_data *data);
int						exec_redirection(t_ast_node *node, t_data *data);
int						handle_exit(char **args, t_data *data);
void					expand_ast(t_data *data, t_ast_node *node);
void					process_quotes_in_ast(t_data *data, t_ast_node *node);
int						calculate_expanded_length(t_data *data, char *str);
void					handle_squote(char *str, char *result, t_expander *exp);
void					handle_dquote(char *str, char *result, t_expander *exp);
int						is_expandable_var(char *str, t_expander *exp);
void					handle_variable_expansion(t_data *data, char *str,
							char *result, t_expander *exp);
char					*extract_var_name(char *str, int *index);
char					*get_env_value(t_env *env, char *key);
void					prepare_cmd_for_execution(t_data *data,
							t_ast_node *node);
void					ft_putstr_error(char *s1, char *s2, char *s3);
void					free_env(t_env *env);
void					start_exec_signals(void);
void					start_parent_exec_signals(void);
int						ft_isquote(char c);
void					print_ast(t_ast_node *node, int depth);

#endif

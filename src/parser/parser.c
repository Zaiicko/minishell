/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:48:47 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/20 19:49:29 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_ast_node	*handle_redirections(t_data *data,
	t_token **tokens, t_ast_node *cmd)
{
	t_ast_node	*result;
	t_node_type	type;
	char		*file;

	result = cmd;
	while (*tokens && ((*tokens)->type == TOKEN_REDIR_IN
			|| (*tokens)->type == TOKEN_REDIR_OUT
			|| (*tokens)->type == TOKEN_APPEND
			|| (*tokens)->type == TOKEN_HEREDOC))
	{
		type = convert_type((*tokens)->type);
		*tokens = (*tokens)->next;
		if (!*tokens || (*tokens)->type != TOKEN_WORD)
			free_all_and_exit_perror(data, "Error\n Missing file name\n");
		file = (*tokens)->value;
		result = new_redir_node(type, result, file);
		if (!result)
			free_all_and_exit_perror(data, "Error\n Node creation failed\n");
		*tokens = (*tokens)->next;
	}
	return (result);
}

t_ast_node	*parse_command(t_data *data, t_token **tokens)
{
	t_ast_node	*cmd_node;
	char		**args;
	int			count;
	int			i;

	if (!*tokens || (*tokens)->type != TOKEN_WORD)
		return (NULL);
	count = count_command_args(*tokens);
	args = (char **)malloc((count + 1) * sizeof(char *));
	if (!args)
		free_all_and_exit_perror(data, "Error\n Malloc failed\n");
	i = 0;
	while (i < count)
	{
		args[i] = ft_strdup((*tokens)->value);
		if (!args[i])
		{
			args[i] = NULL;
			ft_free_tab(args),
			free_all_and_exit_perror(data, "Error\n Malloc failed\n");
		}
		*tokens = (*tokens)->next;
		i++;
	}
	args[i] = NULL;
	cmd_node = new_command_node(args);
	if (!cmd_node)
	{
		ft_free_tab(args),
		free_all_and_exit_perror(data, "Error\n Node creation failed\n");
	}
	ft_free_tab(args);
	return (handle_redirections(data, tokens, cmd_node));
}

t_ast_node	*parse_pipe(t_data *data, t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*node;

	left = parse_command(data, tokens);
	if (!left)
		return (NULL);
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		node = new_pipe_node(left, parse_pipe(data, tokens));
		if (!node)
			free_all_and_exit_perror(data, "Error\n Node creation failed\n");
		return (node);
	}
	return (left);
}

t_ast_node	*parse_logical(t_data *data, t_token **tokens)
{
	t_ast_node	*left;
	t_node_type	type;
	t_ast_node	*node;

	left = parse_pipe(data, tokens);
	if (!left)
		return (NULL);
	if (*tokens && ((*tokens)->type == TOKEN_AND
			|| (*tokens)->type == TOKEN_OR))
	{
		if ((*tokens)->type == TOKEN_AND)
			type = NODE_AND;
		else
			type = NODE_OR;
		*tokens = (*tokens)->next;
		node = new_operator_node(type, left, parse_logical(data, tokens));
		if (!node)
			free_all_and_exit_perror(data, "Error\n Node creation failed\n");
		return (node);
	}
	return (left);
}

t_ast_node	*parse(t_data *data)
{
	t_token		*current;
	t_ast_node	*root;

	if (!data->tokens)
		return (NULL);
	current = data->tokens;
	root = parse_logical(data, &current);
	return (root);
}

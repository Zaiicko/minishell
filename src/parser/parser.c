/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:48:47 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/06 17:36:25 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	fill_command_args(t_data *data, t_token **tokens, char **args,
		int count)
{
	int		i;
	t_token	*current;
	t_token	*next;

	i = 0;
	current = *tokens;
	while (current && i < count)
	{
		next = process_token(data, current, args, &i);
		if (!next)
			break ;
		current = next;
	}
	args[i] = NULL;
	*tokens = current;
}

t_ast_node	*parse_command(t_data *data, t_token **tokens)
{
	t_ast_node	*cmd_node;
	char		**args;
	int			count;

	if (is_redirection_token((*tokens)->type))
	{
		args = (char **)malloc(1 * sizeof(char *));
		if (!args)
			free_all_and_exit_perror(data, "Error\n Malloc failed\n");
		args[0] = NULL;
		cmd_node = new_command_node(args);
		if (!cmd_node)
		{
			free(args);
			free_all_and_exit_perror(data, "Error\n Node creation failed\n");
		}
		return (handle_redirections(data, tokens, cmd_node));
	}
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
		return (NULL);
	count = count_command_args(*tokens);
	args = (char **)malloc((count + 1) * sizeof(char *));
	if (!args)
		free_all_and_exit_perror(data, "Error\n Malloc failed\n");
	fill_command_args(data, tokens, args, count);
	cmd_node = new_command_node(args);
	if (!cmd_node)
	{
		ft_free_tab(args);
		free_all_and_exit_perror(data, "Error\n Node creation failed\n");
	}
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
		{
			free_ast(left);
			free_all_and_exit_perror(data, "Error\n Node creation failed\n");
		}
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
		{
			free_ast(left);
			free_all_and_exit_perror(data, "Error\n Node creation failed\n");
		}
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
	if (!root)
		return (NULL);
	return (root);
}

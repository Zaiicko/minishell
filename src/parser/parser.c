/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:48:47 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/16 19:10:10 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_ast_node	*handle_redirections(t_token **tokens, t_ast_node *cmd)
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
		{
			free_ast(result);
			return (NULL);
		}
		file = (*tokens)->value;
		result = new_redir_node(type, result, file);
		*tokens = (*tokens)->next;
	}
	return (result);
}

t_ast_node	*parse_command(t_token **tokens)
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
		return (NULL);
	i = 0;
	while (i < count)
	{
		args[i] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
		i++;
	}
	args[i] = NULL;
	cmd_node = new_command_node(args);
	return (handle_redirections(tokens, cmd_node));
}

t_ast_node	*parse_pipe(t_token **tokens)
{
	t_ast_node	*left;

	left = parse_command(tokens);
	if (!left)
		return (NULL);
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		return (new_pipe_node(left, parse_pipe(tokens)));
	}
	return (left);
}

t_ast_node	*parse_logical(t_token **tokens)
{
	t_ast_node	*left;
	t_node_type	type;

	left = parse_pipe(tokens);
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
		return (new_operator_node(type, left, parse_logical(tokens)));
	}
	return (left);
}

t_ast_node	*parse(t_token *tokens)
{
	t_token		*current;
	t_ast_node	*root;

	if (!tokens)
		return (NULL);
	current = tokens;
	root = parse_logical(&current);
	return (root);
}

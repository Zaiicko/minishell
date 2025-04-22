/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:49:16 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/21 23:52:07 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	count_command_args(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type == TOKEN_WORD)
	{
		count++;
		current = current->next;
	}
	return (count);
}

t_node_type	convert_type(t_token_type token_type)
{
	if (token_type == TOKEN_REDIR_IN)
		return (NODE_REDIR_IN);
	else if (token_type == TOKEN_REDIR_OUT)
		return (NODE_REDIR_OUT);
	else if (token_type == TOKEN_APPEND)
		return (NODE_APPEND);
	else if (token_type == TOKEN_HEREDOC)
		return (NODE_HEREDOC);
	return (NODE_COMMAND);
}

void	free_ast(t_ast_node *root)
{
	int	i;

	if (!root)
		return ;
	if (root->l)
		free_ast(root->l);
	if (root->r)
		free_ast(root->r);
	if (root->args)
	{
		i = 0;
		while (root->args[i])
		{
			free(root->args[i]);
			i++;
		}
		free(root->args);
	}
	if (root->redir_file)
		free(root->redir_file);
	free(root);
}

t_ast_node	*handle_redirections(t_data *data,
	t_token **tokens, t_ast_node *cmd)
{
	t_ast_node	*result;
	t_node_type	type;

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
			free_all_and_exit_perror(data, "Error\n Missing file name\n");
		}
		result = new_redir_node(type, result, (*tokens)->value);
		if (!result)
		{
			free_ast(cmd);
			free_all_and_exit_perror(data, "Error\n Node creation failed\n");
		}
		*tokens = (*tokens)->next;
	}
	return (result);
}

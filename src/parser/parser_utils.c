/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:49:16 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/16 19:28:52 by zaiicko          ###   ########.fr       */
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

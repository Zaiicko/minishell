/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:49:16 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/06 18:06:00 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	count_command_args(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
			count++;
		else if (current->type == TOKEN_REDIR_IN
			|| current->type == TOKEN_REDIR_OUT || current->type == TOKEN_APPEND
			|| current->type == TOKEN_HEREDOC)
		{
			if (current->next)
				current = current->next;
		}
		else
			break ;
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

static t_ast_node	*apply_single_redirection(t_data *data, t_token *temp,
		t_ast_node *result)
{
	t_node_type	type;
	char		*heredoc_file;

	if (temp->type == TOKEN_HEREDOC)
	{
		heredoc_file = handle_heredoc(temp->value, data);
		if (!heredoc_file)
			return (free_ast(result), free_token_list(&temp), NULL);
		result = new_redir_node(NODE_HEREDOC, result, heredoc_file);
		free(heredoc_file);
	}
	else
	{
		type = convert_type(temp->type);
		result = new_redir_node(type, result, temp->value);
	}
	if (!result)
		free_all_and_exit(data, "Error\n Node creation failed\n");
	return (result);
}

t_ast_node	*handle_redirections(t_data *data, t_token **tokens,
		t_ast_node *cmd)
{
	t_ast_node	*result;
	t_token		*redirs;
	t_token		*temp;
	t_token		*next_token;

	result = cmd;
	redirs = collect_redirections(data, tokens);
	temp = redirs;
	while (temp)
	{
		result = apply_single_redirection(data, temp, result);
		temp = temp->next;
	}
	temp = redirs;
	while (temp)
	{
		next_token = temp->next;
		free(temp->value);
		free(temp);
		temp = next_token;
	}
	return (result);
}

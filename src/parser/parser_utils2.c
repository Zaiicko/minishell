/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:42:44 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/06 18:06:29 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

static t_token	*create_redirection_token(t_data *data, t_token **current)
{
	t_token	*temp;

	temp = malloc(sizeof(t_token));
	if (!temp)
		free_all_and_exit(data, "Error\n Malloc failed\n");
	temp->type = (*current)->type;
	*current = (*current)->next;
	if (!*current || (*current)->type != TOKEN_WORD)
		free_all_and_exit(data, "Error\n Missing filename\n");
	temp->value = ft_strdup((*current)->value);
	if (!temp->value)
		free_all_and_exit(data, "Error\n Malloc failed\n");
	*current = (*current)->next;
	return (temp);
}

t_token	*collect_redirections(t_data *data, t_token **tokens)
{
	t_token	*redirs;
	t_token	*temp;
	t_token	*current;

	redirs = NULL;
	current = *tokens;
	while (current && is_redirection_token(current->type))
	{
		temp = create_redirection_token(data, &current);
		temp->next = redirs;
		redirs = temp;
	}
	*tokens = current;
	return (redirs);
}

t_token	*process_redirection_token(t_token *current)
{
	if (!current || !current->next)
		return (NULL);
	return (current->next->next);
}

t_token	*process_token(t_data *data, t_token *current, char **args, int *i)
{
	if (current->type == TOKEN_WORD)
	{
		args[*i] = ft_strdup(current->value);
		if (!args[*i])
			free_all_and_exit(data, "Error\n Malloc failed\n");
		(*i)++;
		return (current->next);
	}
	else if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT
		|| current->type == TOKEN_APPEND || current->type == TOKEN_HEREDOC)
	{
		if (current->next)
			return (process_redirection_token(current));
	}
	return (NULL);
}

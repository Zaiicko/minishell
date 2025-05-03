/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:42:44 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/03 19:09:36 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

t_token	*collect_redirections(t_data *data, t_token **tokens)
{
	t_token	*current;
	t_token	*redirs;
	t_token	*temp;

	redirs = NULL;
	current = *tokens;
	while (current && is_redirection_token(current->type))
	{
		temp = malloc(sizeof(t_token));
		if (!temp)
			free_all_and_exit_perror(data, "Error\n Malloc failed\n");
		temp->type = current->type;
		temp->next = redirs;
		redirs = temp;
		current = current->next;
		if (!current || current->type != TOKEN_WORD)
			free_all_and_exit_perror(data, "Error\n Missing filename\n");
		temp->value = ft_strdup(current->value);
		if (!temp->value)
			free_all_and_exit_perror(data, "Error\n Malloc failed\n");
		current = current->next;
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
			free_all_and_exit_perror(data, "Error\n Malloc failed\n");
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

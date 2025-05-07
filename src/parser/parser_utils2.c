/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:42:44 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/08 01:13:54 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

static t_token	*create_redirection_token(t_token **current)
{
	t_token	*temp;

	temp = malloc(sizeof(t_token));
	if (!temp)
		return (NULL);
	temp->type = (*current)->type;
	*current = (*current)->next;
	if (!*current || (*current)->type != TOKEN_WORD)
		return (free(temp), NULL);
	temp->value = ft_strdup((*current)->value);
	if (!temp->value)
		return (free(temp), NULL);
	*current = (*current)->next;
	return (temp);
}

int	collect_redirections(t_ast_node *result, t_token **tokens, t_token **redirs)
{
	t_token	*temp;
	t_token	*current;

	*redirs = NULL;
	current = *tokens;
	while (current && is_redirection_token(current->type))
	{
		temp = create_redirection_token(&current);
		if (!temp)
		{
			free_ast(result);
			ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
			ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
			g_exit_status = 258;
			return (2);
		}
		temp->next = *redirs;
		*redirs = temp;
	}
	*tokens = current;
	return (0);
}

t_token	*process_redirection_token(t_token *current)
{
	if (!current || !current->next)
		return (NULL);
	return (current->next->next);
}

t_token	*process_token(t_data *data, t_token *current, char **args, int *i)
{
	(void)data;
	if (current->type == TOKEN_WORD)
	{
		args[*i] = ft_strdup(current->value);
		if (!args[*i])
			return (NULL);
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

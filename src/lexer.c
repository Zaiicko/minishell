/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 01:57:51 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/13 23:19:44 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	handle_redirection(char *input, int i, t_token **head)
{
	if (input[i] == '<')
	{
		if (input[i + 1] == '<')
		{
			add_token_to_list(head, new_token(TOKEN_HEREDOC, "<<"));
			return (i + 2);
		}
		add_token_to_list(head, new_token(TOKEN_REDIR_IN, "<"));
		return (i + 1);
	}
	else if (input[i] == '>')
	{
		if (input[i + 1] == '>')
		{
			add_token_to_list(head, new_token(TOKEN_APPEND, ">>"));
			return (i + 2);
		}
		add_token_to_list(head, new_token(TOKEN_REDIR_OUT, ">"));
		return (i + 1);
	}
	return (i);
}

int	handle_operator(char *input, int i, t_token **head)
{
	if (input[i] == '|')
	{
		if (input[i + 1] == '|')
		{
			add_token_to_list(head, new_token(TOKEN_OR, "||"));
			return (i + 2);
		}
		add_token_to_list(head, new_token(TOKEN_PIPE, "|"));
		return (i + 1);
	}
	else if (input[i] == '<' || input[i] == '>')
	{
		return (handle_redirection(input, i, head));
	}
	if (input[i] == '&' && input[i + 1] == '&')
	{
		add_token_to_list(head, new_token(TOKEN_AND, "&&"));
		return (i + 2);
	}
	return (i);
}

int	update_quotes(int *in_quotes, char *quote_type, char current)
{
	if (*in_quotes == 0)
	{
		*in_quotes = 1;
		*quote_type = current;
	}
	else if (current == *quote_type)
		*in_quotes = 0;
	return (1);
}

int	handle_word(char *input, int i, t_token **head)
{
	int		start;
	int		in_quotes;
	char	quote_type;
	char	*word;

	start = i;
	in_quotes = 0;
	quote_type = 0;
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '"'))
			update_quotes(&in_quotes, &quote_type, input[i]);
		else if (!in_quotes && (is_space(input[i]) || is_operator(input[i])))
			break ;
		i++;
	}
	word = ft_substr(input, start, i - start);
	add_token_to_list(head, new_token(TOKEN_WORD, word));
	free(word);
	return (i);
}

t_token	*tokenize(char *input)
{
	t_token	*head;
	int		i;
	int		in_quotes;

	in_quotes = 0;
	head = NULL;
	i = 0;
	while (input[i])
	{
		if (is_space(input[i]))
			i++;
		else if (is_operator(input[i]) && !in_quotes)
			i = handle_operator(input, i, &head);
		else
			i = handle_word(input, i, &head);
	}
	return (head);
}

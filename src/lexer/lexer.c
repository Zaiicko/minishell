/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 01:57:51 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/06 18:35:16 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	handle_redirection(t_data *data, int i, t_token **head)
{
	if (data->input[i] == '<')
	{
		if (data->input[i + 1] == '<')
		{
			safe_add_token_to_list(data, head, TOKEN_HEREDOC, "<<");
			return (i + 2);
		}
		safe_add_token_to_list(data, head, TOKEN_REDIR_IN, "<");
		return (i + 1);
	}
	else if (data->input[i] == '>')
	{
		if (data->input[i + 1] == '>')
		{
			safe_add_token_to_list(data, head, TOKEN_APPEND, ">>");
			return (i + 2);
		}
		safe_add_token_to_list(data, head, TOKEN_REDIR_OUT, ">");
		return (i + 1);
	}
	return (i);
}

int	handle_operator(t_data *data, int i, t_token **head)
{
	if (data->input[i] == '|')
	{
		safe_add_token_to_list(data, head, TOKEN_PIPE, "|");
		return (i + 1);
	}
	else if (data->input[i] == '<' || data->input[i] == '>')
	{
		return (handle_redirection(data, i, head));
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

int	handle_word(t_data *data, int i, t_token **head)
{
	int		start;
	int		in_quotes;
	char	quote_type;
	char	*word;

	start = i;
	in_quotes = 0;
	quote_type = 0;
	while (data->input[i])
	{
		if ((data->input[i] == '\'' || data->input[i] == '"'))
			update_quotes(&in_quotes, &quote_type, data->input[i]);
		else if (!in_quotes && (is_space(data->input[i])
				|| is_operator(data->input[i])))
			break ;
		i++;
	}
	word = ft_substr(data->input, start, i - start);
	if (!word)
		free_all_and_exit(data, "Error\n Malloc failed\n");
	safe_add_token_to_list(data, head, TOKEN_WORD, word);
	free(word);
	return (i);
}

t_token	*tokenize(t_data *data)
{
	t_token	*head;
	int		i;
	int		in_quotes;

	in_quotes = 0;
	head = NULL;
	i = 0;
	while (data->input[i])
	{
		if (is_space(data->input[i]))
			i++;
		else if (is_operator(data->input[i]) && !in_quotes)
			i = handle_operator(data, i, &head);
		else
			i = handle_word(data, i, &head);
	}
	return (head);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 23:01:08 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/20 17:09:26 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&')
		return (1);
	return (0);
}

void	safe_add_token_to_list(t_data *data, t_token **head,
		t_token_type type, char *value)
{
	t_token	*token;

	token = new_token(type, value);
	if (!token)
		free_all_and_exit_perror(data, "Error\n Malloc failed\n");
	add_token_to_list(head, token);
}

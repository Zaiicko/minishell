/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 22:37:38 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/23 20:18:42 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exit_perror(char *msg)
{
	perror(msg);
	exit(1);
}

void	free_all(t_data *data)
{
	if (data->input)
		free(data->input);
	if (data->tokens)
		free_token_list(&data->tokens);
	if (data->ast)
		free_ast(data->ast);
}

void	free_all_and_exit_perror(t_data *data, char *msg)
{
	free_all(data);
	exit_perror(msg);
}

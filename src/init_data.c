/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:09:02 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/23 17:36:55 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	init_data(t_data *data, char **env)
{
	data->input = NULL;
	data->tokens = NULL;
	data->ast = NULL;
	data->env = init_env(env);
	if (!data->env)
		free_all_and_exit_perror(data, "Error\n Env init failed\n");
}

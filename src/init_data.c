/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:09:02 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/06 18:04:59 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	init_data(t_data *data, char **env)
{
	data->input = NULL;
	data->tokens = NULL;
	data->ast = NULL;
	data->env = NULL;
	data->env = init_env(env, data);
	if (!data->env)
		free_all_and_exit(data, "Error\n Env init failed\n");
}

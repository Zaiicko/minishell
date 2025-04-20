/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:53:58 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/21 00:15:33 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_exit_status = 0;

void	readline_loop(t_data *data)
{
	load_history();
	start_signals();
	while (1)
	{
		data->input = readline("minishell> ");
		if (!data->input || (ft_strncmp(data->input, "exit",
					4) == 0 && data->input[4] == '\0'))
		{
			printf("exit\n");
			break ;
		}
		if (data->input[0])
		{
			add_history(data->input);
			save_history(data);
			data->tokens = tokenize(data);
			if (data->tokens)
			{
				data->ast = parse(data);
				free_token_list(&data->tokens);
			}
		}
		free(data->input);
	}
}

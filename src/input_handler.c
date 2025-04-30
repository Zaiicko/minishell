/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:53:58 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/01 01:16:46 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	process_user_input(t_data *data)
{
	if (data->input && data->input[0])
	{
		add_history(data->input);
		save_history(data);
		data->tokens = tokenize(data);
		if (data->tokens)
		{
			data->ast = parse(data);
			if (data->ast)
			{
				expand_ast(data, data->ast);
				process_quotes_in_ast(data, data->ast);
				execute_ast(data->ast, data);
				free_ast(data->ast);
				data->ast = NULL;
			}
			else
			{
				printf("Parsing error\n");
				g_exit_status = 2;
			}
			if (data->ast)
			{
				free_ast(data->ast);
				data->ast = NULL;
			}
			free_token_list(&data->tokens);
		}
	}
}

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
		process_user_input(data);
		free(data->input);
	}
}

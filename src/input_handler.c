/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:53:58 by zaiicko           #+#    #+#             */
/*   Updated: 2025/03/30 20:56:31 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	readline_loop()
{
	char *input;

	using_history();
	read_history(".readline_history");
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break;	
		}
		if (input[0])
		{
			add_history(input);
			write_history(".readline_history");
		}
		free(input);
	}
	write_history(".readline_history");
	clear_history();
}

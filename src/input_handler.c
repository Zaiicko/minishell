/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:53:58 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/03 18:29:42 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int exit_status = 0;

void	handle_sigint(int sig)
{
	(void)sig;

	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("",0);
	rl_redisplay();
	exit_status = 130;
}

void	start_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	start_exec_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	readline_loop(void)
{
	char *input;

	using_history();
	read_history(".readline_history");
	start_signals();
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

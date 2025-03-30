/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:53:58 by zaiicko           #+#    #+#             */
/*   Updated: 2025/03/30 22:47:48 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int exit_status = 0;

void	config_terminal(void)
{
	struct termios term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	handle_sigint(int sig)
{
	(void)sig;

	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("",0);
	rl_redisplay();
	exit_status = 130;
}

void	init_signals(void)
{
	signal(SIGINT, handle_sigint);
}

void	readline_loop(void)
{
	char *input;

	using_history();
	read_history(".readline_history");
	config_terminal();
	init_signals();
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

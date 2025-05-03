/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 21:21:12 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/03 04:06:26 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
}

void	start_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_sigint_parent_exec(int sig)
{
	(void)sig;
}

void	start_parent_exec_signals(void)
{
	signal(SIGINT, handle_sigint_parent_exec);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_sigint_exec(int sig)
{
	(void)sig;
}

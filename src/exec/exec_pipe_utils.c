/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:52:48 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/06 16:55:08 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	exec_pipe_child_left(int pipefd[2], t_ast_node *node_side, t_data *data)
{
	int	exit_code;

	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		free_all(data);
		exit(1);
	}
	close(pipefd[1]);
	exit_code = execute_ast(node_side, data);
	free_all(data);
	exit(exit_code);
}

void	exec_pipe_child_right(int pipefd[2], t_ast_node *node_side,
		t_data *data)
{
	int	exit_code;

	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		free_all(data);
		exit(1);
	}
	close(pipefd[0]);
	exit_code = execute_ast(node_side, data);
	free_all(data);
	exit(exit_code);
}

int	handle_pipe_signal(int status2)
{
	if (WIFSIGNALED(status2))
	{
		if (WTERMSIG(status2) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status2) == SIGQUIT)
			write(1, "Quit: 3\n", 8);
		g_exit_status = 128 + WTERMSIG(status2);
	}
	else
		g_exit_status = WEXITSTATUS(status2);
	return (g_exit_status);
}

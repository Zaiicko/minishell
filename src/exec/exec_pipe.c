/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:27:23 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/06 16:58:01 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	exec_pipe_child(int pipefd[2], t_ast_node *node_side, t_data *data,
		int is_left_side)
{
	start_exec_signals();
	if (is_left_side)
		exec_pipe_child_left(pipefd, node_side, data);
	else
		exec_pipe_child_right(pipefd, node_side, data);
}

static pid_t	create_child(int pipefd[2], t_ast_node *node, t_data *data,
		int is_first)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (is_first)
			exec_pipe_child(pipefd, node->l, data, 1);
		else
			exec_pipe_child(pipefd, node->r, data, 0);
	}
	return (pid);
}

static int	create_pipe_processes(int pipefd[2], t_ast_node *node, t_data *data)
{
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	pid1 = create_child(pipefd, node, data, 1);
	if (pid1 == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	pid2 = create_child(pipefd, node, data, 0);
	if (pid2 == -1)
		return (close(pipefd[0]), close(pipefd[1]), waitpid(pid1, NULL, 0), 1);
	close(pipefd[0]);
	close(pipefd[1]);
	start_parent_exec_signals();
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	start_signals();
	return (handle_pipe_signal(status2));
}

int	exec_pipe(t_ast_node *node, t_data *data)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (perror("pipe"), g_exit_status = 1);
	g_exit_status = create_pipe_processes(pipefd, node, data);
	return (g_exit_status);
}

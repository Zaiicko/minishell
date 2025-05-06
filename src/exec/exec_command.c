/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:26:07 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/06 17:22:18 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_command_error(char *cmd, int error_code, t_data *data)
{
	if (error_code == ENOENT)
	{
		ft_putstr_error("minishell: ", cmd, ": command not found");
		free(cmd);
		free_all(data);
		exit(127);
	}
	else if (error_code == EACCES)
	{
		ft_putstr_error("minishell: ", cmd, ": Permission denied");
		free(cmd);
		free_all(data);
		exit(126);
	}
	else
	{
		write(STDERR_FILENO, "minishell: ", 11);
		ft_putstr_error(cmd, ": ", strerror(error_code));
		free(cmd);
		free_all(data);
		exit(1);
	}
}

static int	handle_command_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit: 3\n", 8);
		g_exit_status = 128 + WTERMSIG(status);
	}
	else
		g_exit_status = WEXITSTATUS(status);
	return (g_exit_status);
}

int	exec_command(t_ast_node *node, t_data *data)
{
	pid_t	pid;
	int		status;

	if (!node->args || !node->args[0] || node->args[0][0] == '\0')
		return (g_exit_status = 0);
	if (is_builtin(node->args[0]))
		return (exec_builtin(node->args, data->env, data));
	pid = fork();
	if (pid == 0)
		exec_command_child(node, data);
	else if (pid > 0)
	{
		start_parent_exec_signals();
		waitpid(pid, &status, 0);
		handle_command_status(status);
		start_signals();
	}
	else
	{
		perror("fork");
		g_exit_status = 1;
	}
	return (g_exit_status);
}

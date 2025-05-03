/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:21:58 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/03 16:31:21 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_ast(t_ast_node *node, t_data *data)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		return (exec_command(node, data));
	else if (node->type == NODE_PIPE)
		return (exec_pipe(node, data));
	else if (node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT
		|| node->type == NODE_APPEND || node->type == NODE_HEREDOC)
		return (exec_redirection(node, data));
	else if (node->type == NODE_AND)
	{
		if (execute_ast(node->l, data) == 0)
			return (execute_ast(node->r, data));
		return (g_exit_status);
	}
	else if (node->type == NODE_OR)
	{
		if (execute_ast(node->l, data) != 0)
			return (execute_ast(node->r, data));
		return (g_exit_status);
	}
	return (0);
}

// int	exec_command(t_ast_node *node, t_data *data)
// {
// 	pid_t	pid;
// 	int		status;

// 	if (!node->args || !node->args[0] || node->args[0][0] == '\0')
// 		return (g_exit_status = 0);
// 	if (is_builtin(node->args[0]))
// 		return (exec_builtin(node->args, data->env, data));
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		start_exec_signals();
// 		execvp(node->args[0], node->args);
// 		if (errno == ENOENT)
// 		{
// 			ft_putstr_error("minishell: ", node->args[0],
// 				": command not found");
// 			exit(127);
// 		}
// 		else if (errno == EACCES)
// 		{
// 			ft_putstr_error("minishell: ", node->args[0],
// 				": Permission denied");
// 			exit(126);
// 		}
// 		else
// 		{
// 			ft_putstr_error("minishell: ", node->args[0], ": ");
// 			ft_putstr_error(strerror(errno), NULL, NULL);
// 			exit(1);
// 		}
// 	}
// 	else if (pid > 0)
// 	{
// 		start_parent_exec_signals();
// 		waitpid(pid, &status, 0);
// 		if (WIFSIGNALED(status))
// 		{
// 			if (WTERMSIG(status) == SIGINT)
// 				write(1, "\n", 1);
// 			else if (WTERMSIG(status) == SIGQUIT)
// 				write(1, "Quit: 3\n", 8);
// 			g_exit_status = 128 + WTERMSIG(status);
// 		}
// 		else
// 			g_exit_status = WEXITSTATUS(status);
// 		start_signals();
// 		return (g_exit_status);
// 	}
// 	else
// 	{
// 		perror("fork");
// 		return (g_exit_status = 1);
// 	}
// }

// int	exec_pipe(t_ast_node *node, t_data *data)
// {
// 	int	pipefd[2];

// 	pid_t pid1, pid2;
// 	int status1, status2;
// 	if (pipe(pipefd) == -1)
// 		return (perror("pipe"), g_exit_status = 1);
// 	pid1 = fork();
// 	if (pid1 == -1)
// 	{
// 		perror("fork");
// 		close(pipefd[0]);
// 		close(pipefd[1]);
// 		return (g_exit_status = 1);
// 	}
// 	if (pid1 == 0)
// 	{
// 		start_exec_signals();
// 		close(pipefd[0]);
// 		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
// 		{
// 			perror("dup2");
// 			exit(1);
// 		}
// 		close(pipefd[1]);
// 		exit(execute_ast(node->l, data));
// 	}
// 	pid2 = fork();
// 	if (pid2 == -1)
// 	{
// 		perror("fork");
// 		close(pipefd[0]);
// 		close(pipefd[1]);
// 		waitpid(pid1, NULL, 0);
// 		return (g_exit_status = 1);
// 	}
// 	if (pid2 == 0)
// 	{
// 		start_exec_signals();
// 		close(pipefd[1]);
// 		if (dup2(pipefd[0], STDIN_FILENO) == -1)
// 		{
// 			perror("dup2");
// 			exit(1);
// 		}
// 		close(pipefd[0]);
// 		exit(execute_ast(node->r, data));
// 	}
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	start_parent_exec_signals();
// 	waitpid(pid1, &status1, 0);
// 	waitpid(pid2, &status2, 0);
// 	start_signals();
// 	if (WIFSIGNALED(status2))
// 	{
// 		if (WTERMSIG(status2) == SIGINT)
// 			write(1, "\n", 1);
// 		else if (WTERMSIG(status2) == SIGQUIT)
// 			write(1, "Quit: 3\n", 8);
// 		g_exit_status = 128 + WTERMSIG(status2);
// 	}
// 	else
// 		g_exit_status = WEXITSTATUS(status2);
// 	return (g_exit_status);
// }

// int	exec_redirection(t_ast_node *node, t_data *data)
// {
// 	int fd;
// 	int saved_fd;
// 	int std_fd;
// 	int status = 0;

// 	if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC)
// 		std_fd = STDIN_FILENO;
// 	else
// 		std_fd = STDOUT_FILENO;
// 	saved_fd = dup(std_fd);
// 	if (saved_fd < 0)
// 		return (perror("dup"), g_exit_status = 1);
// 	if (node->type == NODE_REDIR_IN)
// 		fd = open(node->redir_file, O_RDONLY);
// 	else if (node->type == NODE_REDIR_OUT)
// 		fd = open(node->redir_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	else if (node->type == NODE_APPEND)
// 		fd = open(node->redir_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	else if (node->type == NODE_HEREDOC)
// 		fd = open(node->redir_file, O_RDONLY);
// 	else
// 		fd = -1;
// 	if (fd < 0)
// 	{
// 		ft_putstr_error("minishell: ", node->redir_file, ": ");
// 		ft_putstr_error(strerror(errno), NULL, NULL);
// 		close(saved_fd);
// 		g_exit_status = 1;
// 		return (g_exit_status);
// 	}
// 	if (dup2(fd, std_fd) < 0)
// 	{
// 		perror("dup2");
// 		close(fd);
// 		close(saved_fd);
// 		g_exit_status = 1;
// 		return (g_exit_status);
// 	}
// 	close(fd);
// 	if (node->l)
// 		status = execute_ast(node->l, data);
// 	dup2(saved_fd, std_fd);
// 	close(saved_fd);
// 	g_exit_status = status;
// 	return (status);
// }
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:21:58 by nicleena          #+#    #+#             */
/*   Updated: 2025/04/24 15:05:28 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_ast(t_ast_node *node, t_data *data)
{
	if (!node)
		return (1);
	if (node->type == NODE_COMMAND)
		return (exec_command(node, data));
	if (node->type == NODE_PIPE)
		return (exec_pipe(node, data));
	// if (node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT
	// 	|| node->type == NODE_APPEND || node->type == NODE_HEREDOC)
	// 	return (exec_redirection(node, data));
	if (node->type == NODE_AND)
	{
		if (execute_ast(node->l, data) == 0)
			return (execute_ast(node->r, data));
		return (1);
	}
	if (node->type == NODE_OR)
	{
		if (execute_ast(node->l, data) != 0)
			return (execute_ast(node->r, data));
		return (0);
	}
	return (1);
}
int	exec_command(t_ast_node *node, t_data *data)
{
	pid_t	pid;
	int		status;

	if (!node->args || !node->args[0])
		return (1);
	if (is_builtin(node->args[0]))
		return (exec_builtin(node->args, data->env));
	pid = fork();
	if (pid == 0)
	{
		execvp(node->args[0], node->args);
		perror("execvp");
		exit(1);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	else
	{
		perror("fork");
		return (1);
	}
}

int exec_pipe(t_ast_node *node, t_data *data)
{
    int pipefd[2];
    pid_t pid1;
    pid_t pid2;
    int status;

    if (pipe(pipefd) == -1)
        return (perror("pipe"), 1);
    pid1 = fork();
    if (pid1 == -1)
        return (perror("fork"), 1);
    if (pid1 == 0)
        exec_pipe_child(pipefd, node->l, data, STDOUT_FILENO);
    pid2 = fork();
    if (pid2 == -1)
        return (perror("fork"), 1);
    if (pid2 == 0)
        exec_pipe_child(pipefd, node->r, data, STDIN_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    return (WEXITSTATUS(status));
}
void exec_pipe_child(int pipefd[2], t_ast_node *node, t_data *data, int fd)
{
    if (fd == STDOUT_FILENO)
        dup2(pipefd[1], fd);
    else
        dup2(pipefd[0], fd);
    close(pipefd[0]);
    close(pipefd[1]);
    exec_command(node, data);
    exit(0);
}


// int exec_redirection(t_ast_node *node)

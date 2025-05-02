/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:21:58 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/02 14:33:37 by nicleena         ###   ########.fr       */
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
	if (node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT
		|| node->type == NODE_APPEND || node->type == NODE_HEREDOC)
		return (exec_redirection(node, data));
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

int exec_command(t_ast_node *node, t_data *data)
{
    pid_t pid;
    int status;

    if (!node->args || !node->args[0])
        return (g_exit_status = 0);
        
    if (is_builtin(node->args[0]))
    {
        g_exit_status = exec_builtin(node->args, data->env);
        return (g_exit_status);
    }
    
    pid = fork();
    if (pid == 0)
    {
        execvp(node->args[0], node->args);
        if (errno == ENOENT)
            g_exit_status = 127;
        else if (errno == EACCES)
            g_exit_status = 126;
        else
            g_exit_status = 1;
        fprintf(stderr, "minishell: %s: %s\n", node->args[0], strerror(errno));
        exit(g_exit_status);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status))
            g_exit_status = 128 + WTERMSIG(status);
        else
            g_exit_status = WEXITSTATUS(status);
        return (g_exit_status);
    }
    else
    {
        perror("fork");
        return (g_exit_status = 1);
    }
}

int exec_pipe(t_ast_node *node, t_data *data)
{
    int pipefd[2];
    pid_t pid1;
    pid_t pid2;
    int status1;
    int status2;

    if (pipe(pipefd) == -1)
        return (perror("pipe"), g_exit_status = 1);
    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return (g_exit_status = 1);
    }
    if (pid1 == 0)
        exec_pipe_child(pipefd, node->l, data, STDOUT_FILENO);   
    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid1, NULL, 0);
        return (g_exit_status = 1);
    }
    if (pid2 == 0)
        exec_pipe_child(pipefd, node->r, data, STDIN_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    if (WIFSIGNALED(status2))
        g_exit_status = 128 + WTERMSIG(status2);
    else
        g_exit_status = WEXITSTATUS(status2);
        
    return (g_exit_status);
}

void exec_pipe_child(int pipefd[2], t_ast_node *node, t_data *data, int fd)
{
    if (fd == STDOUT_FILENO)
        dup2(pipefd[1], fd);
    else
        dup2(pipefd[0], fd);
    close(pipefd[0]);
    close(pipefd[1]);
    
    exit(execute_ast(node, data));
}

int exec_redirection(t_ast_node *node, t_data *data)
{
    int fd;
    int saved_fd;
    int status;
    int std_fd;

    if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC)
        std_fd = STDIN_FILENO;
    else
        std_fd = STDOUT_FILENO;
    saved_fd = dup(std_fd);
    if (saved_fd < 0)
        return (perror("dup"), g_exit_status = 1);
    if (node->type == NODE_REDIR_IN)
        fd = open(node->redir_file, O_RDONLY);
    else if (node->type == NODE_REDIR_OUT)
        fd = open(node->redir_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (node->type == NODE_APPEND)
        fd = open(node->redir_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (node->type == NODE_HEREDOC)
        fd = open(node->redir_file, O_RDONLY);
    else
        fd = -1;  
    if (fd < 0)
    {
        fprintf(stderr, "minishell: %s: %s\n", node->redir_file, strerror(errno));
        close(saved_fd);
        return (g_exit_status = 1);
    }
    if (dup2(fd, std_fd) < 0)
    {
        perror("dup2");
        close(fd);
        close(saved_fd);
        return (g_exit_status = 1);
	}
    close(fd);
    status = execute_ast(node->l, data);
    dup2(saved_fd, std_fd);
    close(saved_fd);
    g_exit_status = status;
    return (g_exit_status);
}

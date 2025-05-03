/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:27:52 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/03 16:30:55 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	open_redir_file(t_ast_node *node)
{
	int	fd;

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
	return (fd);
}

static int	handle_redir_error(t_ast_node *node, int saved_fd)
{
	ft_putstr_error("minishell: ", node->redir_file, ": ");
	ft_putstr_error(strerror(errno), NULL, NULL);
	close(saved_fd);
	g_exit_status = 1;
	return (g_exit_status);
}

static int	apply_redirection(int fd, int std_fd, int saved_fd)
{
	if (dup2(fd, std_fd) < 0)
	{
		perror("dup2");
		close(fd);
		close(saved_fd);
		g_exit_status = 1;
		return (0);
	}
	close(fd);
	return (1);
}

int	exec_redirection(t_ast_node *node, t_data *data)
{
	int	fd;
	int	saved_fd;
	int	std_fd;
	int	status;

	if (node->type == NODE_REDIR_IN || node->type == NODE_HEREDOC)
		std_fd = STDIN_FILENO;
	else
		std_fd = STDOUT_FILENO;
	saved_fd = dup(std_fd);
	if (saved_fd < 0)
		return (perror("dup"), g_exit_status = 1);
	fd = open_redir_file(node);
	if (fd < 0)
		return (handle_redir_error(node, saved_fd));
	if (!apply_redirection(fd, std_fd, saved_fd))
		return (g_exit_status);
	status = 0;
	if (node->l)
		status = execute_ast(node->l, data);
	dup2(saved_fd, std_fd);
	close(saved_fd);
	g_exit_status = status;
	return (status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:21:58 by nicleena          #+#    #+#             */
/*   Updated: 2025/04/23 16:24:50 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	execute_ast(t_ast_node *node)
{
	if (!node)
		return (1);
	if (node->type == NODE_COMMAND)
		return (exec_command(node));
	if (node->type == NODE_PIPE)
		return (exec_pipe(node));
	if (node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT
		|| node->type == NODE_APPEND || node->type == NODE_HEREDOC)
		return (exec_redirection(node));
	if (node->type == NODE_AND)
	{
		if (execute_ast(node->l) == 0)
			return (execute_ast(node->r));
		return (1);
	}
	if (node->type == NODE_OR)
	{
		if (execute_ast(node->l) != 0)
			return (execute_ast(node->r));
		return (0);
	}
	return (1);
}
int	exec_command(t_ast_node *node)
{
	pid_t	pid;
	int		status;

	if (!node->args || !node->args[0])
		return (1);
	if (is_builtin(node->args[0]))
		return (exec_builtin(node->args));
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

// int	exec_pipe(t_ast_node *node)
// int exec_redirection(t_ast_node *node)


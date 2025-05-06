/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:21:58 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/06 18:24:37 by zaiicko          ###   ########.fr       */
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
	return (0);
}

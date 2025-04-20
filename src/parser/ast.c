/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 13:09:03 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/20 03:36:08 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_ast_node	*new_node(t_node_type type)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->redir_file = NULL;
	node->r = NULL;
	node->l = NULL;
	return (node);
}

t_ast_node	*new_command_node(char	**args)
{
	t_ast_node	*node;

	node = new_node(NODE_COMMAND);
	if (!node)
		return (NULL);
	node->args = args;
	return (node);
}

t_ast_node	*new_pipe_node(t_ast_node *l_cmd, t_ast_node *r_cmd)
{
	t_ast_node	*node;

	node = new_node(NODE_PIPE);
	if (!node)
		return (NULL);
	node->r = r_cmd;
	node->l = l_cmd;
	return (node);
}

t_ast_node	*new_redir_node(t_node_type type, t_ast_node *cmd, char *target)
{
	t_ast_node	*node;

	if (type != NODE_REDIR_IN && type != NODE_REDIR_OUT
		&& type != NODE_APPEND && type != NODE_HEREDOC)
		return (NULL);
	node = new_node(type);
	if (!node)
		return (NULL);
	node->l = cmd;
	node->redir_file = ft_strdup(target);
	if (!node->redir_file)
	{
		free(node);
		return (NULL);
	}
	return (node);
}

t_ast_node	*new_operator_node(t_node_type type, t_ast_node *l, t_ast_node *r)
{
	t_ast_node	*node;

	if (type != NODE_AND && type != NODE_OR)
		return (NULL);
	node = new_node(type);
	if (!node)
		return (NULL);
	node->l = l;
	node->r = r;
	return (node);
}

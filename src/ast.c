/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 13:09:03 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/03 01:59:48 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_ast_node	*new_node(t_node_type type)
{
	t_ast_node	*node = malloc(sizeof(t_ast_node));
	if (!node)
		return NULL;
	node->type = type;
	node->args = NULL;
	node->redir_file = NULL;
	node->r = NULL;
	node->l = NULL;
	return (node);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:48:58 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/06 18:06:38 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_ast_node	*parse_firstredirect_command(t_data *data, t_token **tokens)
{
	t_ast_node	*cmd_node;
	char		**args;

	args = (char **)malloc(1 * sizeof(char *));
	if (!args)
		free_all_and_exit(data, "Error\n Malloc failed\n");
	args[0] = NULL;
	cmd_node = new_command_node(args);
	if (!cmd_node)
	{
		free(args);
		free_all_and_exit(data, "Error\n Node creation failed\n");
	}
	return (handle_redirections(data, tokens, cmd_node));
}

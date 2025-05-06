/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_child_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:37:18 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/06 16:39:31 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*handle_direct_path(char *cmd)
{
	char	*result;

	if (ft_strchr(cmd, '/'))
	{
		result = ft_strdup(cmd);
		return (result);
	}
	return (NULL);
}

char	*handle_path_not_found(char *cmd)
{
	char	*result;

	result = ft_strdup(cmd);
	return (result);
}

void	handle_exec_error(char *cmd_path, char *cmd_dup, t_data *data)
{
	free(cmd_path);
	handle_command_error(cmd_dup, errno, data);
}

void	setup_execute(t_ast_node *node, t_data *data, char **cmd_path,
		char **cmd_dup)
{
	*cmd_dup = ft_strdup(node->args[0]);
	if (!*cmd_dup)
	{
		free_all(data);
		exit(1);
	}
	*cmd_path = find_command_path(node->args[0], data->env);
	if (!*cmd_path)
		handle_command_error(*cmd_dup, ENOMEM, data);
}

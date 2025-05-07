/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_child_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:37:18 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/07 14:38:01 by zaiicko          ###   ########.fr       */
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

int	is_directory(char *cmd_path, char *cmd_dup, t_data *data)
{
	struct stat	st;

	if (ft_strchr(cmd_path, '/') && stat(cmd_path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			ft_putstr_error("minishell: ", cmd_dup, ": Is a directory");
			free(cmd_path);
			if (cmd_dup)
				free(cmd_dup);
			return (free_all(data), exit(126), 1);
		}
		else if (access(cmd_path, X_OK) == -1)
		{
			ft_putstr_error("minishell: ", cmd_dup, ": Permission denied");
			free(cmd_path);
			if (cmd_dup)
				free(cmd_dup);
			return (free_all(data), exit(126), 1);
		}
		else
			return (free(cmd_path), 1);
	}
	return (0);
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

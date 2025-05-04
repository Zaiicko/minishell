/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_child.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:04:07 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/05 01:27:59 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	fill_env_array(char **env_array, t_env_var *current, int i)
{
	char	*tmp;

	while (current)
	{
		tmp = ft_strjoin(current->key, "=");
		if (!tmp)
			return (0);
		env_array[i] = ft_strjoin(tmp, current->value);
		free(tmp);
		if (!env_array[i])
			return (0);
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (1);
}

static char	**env_to_array(t_env *env)
{
	t_env_var	*current;
	char		**env_array;
	int			count;
	int			i;

	count = 0;
	current = env->head;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = (char *)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current = env->head;
	if (!fill_env_array(env_array, current, i))
	{
		ft_free_tab(env_array);
		return (NULL);
	}
	return (env_array);
}

static char	*search_in_paths(char **paths, char *cmd)
{
	char	*path_prefix;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		path_prefix = ft_strjoin(paths[i], "/");
		if (!path_prefix)
			return (NULL);
		full_path = ft_strjoin(path_prefix, cmd);
		free(path_prefix);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

static char	*find_command_path(char *cmd, t_env *env)
{
	char	*path;
	char	**paths;
	char	*cmd_path;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path = get_env_value(env, "PATH");
	if (!path)
		return (ft_strdup(cmd));
	paths = ft_split(path, ':');
	free(path);
	if (!paths)
		return (ft_strdup(cmd));
	cmd_path = search_in_paths(paths, cmd);
	ft_free_tab(paths);
	if (!cmd_path)
		return (ft_strdup(cmd));
	return (cmd_path);
}

void	exec_command_child(t_ast_node *node, t_data *data)
{
	char	*cmd_path;
	char	**env_array;

	start_exec_signals();
	cmd_path = find_command_path(node->args[0], data->env);
	if (!cmd_path)
		handle_command_error(node->args[0], ENOMEM, data);
	env_array = env_to_array(data->env);
	if (!env_array)
	{
		free(cmd_path);
		handle_command_error(node->args[0], ENOMEM, data);
	}
	execve(cmd_path, node->args, env_array);
	free(cmd_path);
	ft_free_tab(env_array);
	handle_command_error(node->args[0], errno, data);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_child.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:04:07 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/07 14:38:34 by zaiicko          ###   ########.fr       */
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
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
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

char	*find_command_path(char *cmd, t_env *env)
{
	char	*path;
	char	**paths;
	char	*cmd_path;
	char	*direct_result;

	direct_result = handle_direct_path(cmd);
	if (direct_result)
		return (direct_result);
	path = get_env_value(env, "PATH");
	if (!path)
		return (handle_path_not_found(cmd));
	paths = ft_split(path, ':');
	free(path);
	if (!paths)
		return (handle_path_not_found(cmd));
	cmd_path = search_in_paths(paths, cmd);
	ft_free_tab(paths);
	if (!cmd_path)
		return (handle_path_not_found(cmd));
	return (cmd_path);
}

void	exec_command_child(t_ast_node *node, t_data *data)
{
	char	*cmd_path;
	char	**env_array;
	char	**args_copy;
	char	*cmd_dup;

	start_exec_signals();
	setup_execute(node, data, &cmd_path, &cmd_dup);
	env_array = env_to_array(data->env);
	if (!env_array)
	{
		free(cmd_path);
		handle_command_error(cmd_dup, ENOMEM, data);
	}
	args_copy = ft_tabdup(node->args);
	if (!args_copy)
	{
		free(cmd_path);
		ft_free_tab(env_array);
		handle_command_error(cmd_dup, ENOMEM, data);
	}
	execve(cmd_path, args_copy, env_array);
	is_directory(cmd_path, cmd_dup, data);
	return (free(cmd_path), ft_free_tab(env_array),
		ft_free_tab(args_copy), handle_command_error(cmd_dup, errno, data));
}

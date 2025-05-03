/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:31:22 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/03 19:16:44 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"));
}

static int	exec_builtin_cd_export_unset(char **args, t_env *env, t_data *data)
{
	if (!ft_strcmp(args[0], "cd"))
	{
		ft_cd(args, env, data);
		return (1);
	}
	else if (!ft_strcmp(args[0], "export") && args[1])
	{
		return (ft_export_with_args(env, args, data));
	}
	else if (!ft_strcmp(args[0], "export"))
	{
		ft_export(env);
		return (0);
	}
	else if (!ft_strcmp(args[0], "unset") && args[1])
		ft_unset(args, env);
	else
		return (0);
	return (1);
}

int	exec_builtin(char **args, t_env *env, t_data *data)
{
	if (!args || !args[0])
		return (0);
	if (!ft_strcmp(args[0], "echo"))
	{
		ft_echo(args);
		return (0);
	}
	else if (!ft_strcmp(args[0], "pwd"))
	{
		ft_pwd();
		return (g_exit_status);
	}
	else if (!ft_strcmp(args[0], "env"))
	{
		ft_env(env);
		return (0);
	}
	else if (!ft_strcmp(args[0], "exit"))
	{
		return (handle_exit(args, data));
	}
	else
		return (exec_builtin_cd_export_unset(args, env, data));
}

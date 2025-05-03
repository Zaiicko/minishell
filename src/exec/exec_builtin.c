/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:31:22 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/03 18:34:06 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_builtin(char *cmd)
{
	return (!strcmp(cmd, "cd") || !strcmp(cmd, "echo") || !strcmp(cmd, "pwd")
		|| !strcmp(cmd, "env") || !strcmp(cmd, "export") || !strcmp(cmd,
			"unset") || !strcmp(cmd, "exit"));
}

static int	exec_builtin_cd_export_unset(char **args, t_env *env, t_data *data)
{
	char	*equal;

	if (!strcmp(args[0], "cd"))
	{
		ft_cd(args, env, data);
		return (1);
	}
	else if (!strcmp(args[0], "export") && args[1])
	{
		equal = strchr(args[1], '=');
		if (equal)
		{
			*equal = '\0';
			ft_setenv(env, args[1], equal + 1, data);
		}
	}
	else if (!strcmp(args[0], "unset") && args[1])
		ft_unset(args, env);
	else
		return (0);
	return (1);
}

int	exec_builtin(char **args, t_env *env, t_data *data)
{
	if (!args || !args[0])
		return (0);
	if (!strcmp(args[0], "echo"))
	{
		ft_echo(args);
		return (0);
	}
	else if (!strcmp(args[0], "pwd"))
	{
		ft_pwd();
		return (g_exit_status);
	}
	else if (!strcmp(args[0], "env"))
	{
		ft_env(env);
		return (0);
	}
	else if (!strcmp(args[0], "exit"))
	{
		return (handle_exit(args, data));
	}
	else
		return (exec_builtin_cd_export_unset(args, env, data));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:19:44 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/03 17:57:12 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	update_pwd_vars(char *oldpwd, t_env *env, t_data *data)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
	if (newpwd)
	{
		ft_setenv(env, "OLDPWD", oldpwd, data);
		ft_setenv(env, "PWD", newpwd, data);
		free(newpwd);
		g_exit_status = 0;
	}
	else
	{
		perror("getcwd");
		g_exit_status = 1;
	}
}

void	ft_tilde(t_env *env, t_data *data)
{
	char	*home;
	char	*oldpwd;

	home = get_env_value(env, "HOME");
	if (!home)
	{
		ft_putstr_error("minishell: cd: ", "HOME not set", NULL);
		g_exit_status = 1;
		return ;
	}
	oldpwd = getcwd(NULL, 0);
	if (chdir(home) == -1)
	{
		ft_putstr_error("minishell: cd: ", home, ": ");
		ft_putstr_error(strerror(errno), NULL, NULL);
		g_exit_status = 1;
		free(oldpwd);
	}
	else
	{
		update_pwd_vars(oldpwd, env, data);
		free(oldpwd);
	}
}

void	ft_cd_oldpwd(t_env *env, t_data *data)
{
	char	*current;
	char	*oldpwd;

	oldpwd = get_env_value(env, "OLDPWD");
	if (!oldpwd)
	{
		ft_putstr_error("minishell: cd: ", "OLDPWD not set", NULL);
		g_exit_status = 1;
		return ;
	}
	current = getcwd(NULL, 0);
	if (chdir(oldpwd) == -1)
	{
		ft_putstr_error("minishell: cd: ", oldpwd, ": ");
		ft_putstr_error(strerror(errno), NULL, NULL);
		g_exit_status = 1;
		free(current);
	}
	else
	{
		update_pwd_vars(current, env, data);
		free(current);
		printf("%s\n", oldpwd);
	}
}

// 3. Corriger la définition de ft_cd
int	ft_cd(char **args, t_env *env, t_data *data)
{
	char	*path;
	char	*oldpwd;

	g_exit_status = 0;
	if (!args[1])
	{
		ft_tilde(env, data);
		return (g_exit_status);
	}
	path = args[1];
	if (ft_strncmp(path, "~", 1) == 0)
	{
		ft_tilde(env, data);
		return (g_exit_status);
	}
	else if (ft_strncmp(path, "-", 1) == 0)
	{
		ft_cd_oldpwd(env, data);
		return (g_exit_status);
	}
	else if (ft_strncmp(path, "/", 1) == 0)
	{
		oldpwd = getcwd(NULL, 0);
		if (chdir(path) == -1)
		{
			ft_putstr_error("minishell: cd: ", path, ": ");
			ft_putstr_error(strerror(errno), NULL, NULL);
			g_exit_status = 1;
			free(oldpwd);
		}
		else
		{
			update_pwd_vars(oldpwd, env, data);
			free(oldpwd);
		}
		return (g_exit_status);
	}
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		ft_putstr_error("minishell: cd: ", path, ": ");
		ft_putstr_error(strerror(errno), NULL, NULL);
		g_exit_status = 1;
		free(oldpwd);
	}
	else
	{
		update_pwd_vars(oldpwd, env, data);
		free(oldpwd);
	}
	return (g_exit_status);
}

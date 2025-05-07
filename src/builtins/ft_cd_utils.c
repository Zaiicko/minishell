/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:25:43 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/07 14:39:11 by zaiicko          ###   ########.fr       */
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

int	handle_absolute_path(char *path, char *oldpwd, t_env *env, t_data *data)
{
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

int	handle_relative_path(char *path, t_env *env, t_data *data)
{
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 1);
		ft_putstr_error(path, ": ", strerror(errno));
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

void	handle_cd_oldpwd_error(char *oldpwd, char *current)
{
	ft_putstr_error("minishell: cd: ", oldpwd, ": ");
	ft_putstr_error(strerror(errno), NULL, NULL);
	g_exit_status = 1;
	free(current);
	free(oldpwd);
}

void	change_to_oldpwd(char *oldpwd, char *current, t_env *env, t_data *data)
{
	if (chdir(oldpwd) == -1)
		handle_cd_oldpwd_error(oldpwd, current);
	else
	{
		update_pwd_vars(current, env, data);
		free(current);
		printf("%s\n", oldpwd);
		free(oldpwd);
	}
}

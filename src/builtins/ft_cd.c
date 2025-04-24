/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:19:44 by nicleena          #+#    #+#             */
/*   Updated: 2025/04/24 17:28:18 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_cd(char *path)
{
	char	*oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		perror("cd");
		g_exit_status = 1;
		free(oldpwd);
		return ;
	}
	newpwd = getcwd(NULL, 0);
	if (newpwd)
	{
		setenv("OLDPWD", oldpwd, 1);
		setenv("PWD", newpwd, 1);
		free(newpwd);
	}
	else
	{
		perror("getcwd");
		g_exit_status = 1;
	}
	free(oldpwd);
}

void	ft_cd_oldpwd(void)
{
	char	*oldpwd;

	oldpwd = getenv("OLDPWD");
	if (oldpwd)
		ft_cd(oldpwd);
	else
		printf("%s\n", "cd: OLDPWD not set\n");
}

void	ft_tilde(void)
{
	char	*home;

	home = getenv("HOME");
	if (home)
		ft_cd(home);
	else
		printf("%s\n", "cd: HOME not set\n");
}

void	ft_dotdot(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
		printf("%s\n", cwd);
	else
		perror("getcwd");
	free(cwd);
}

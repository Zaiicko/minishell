/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:18:31 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/03 16:14:41 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		g_exit_status = 0;
	}
	else
	{
		perror("getcwd");
		g_exit_status = 1;
	}
}

void	ft_oldpwd(void)
{
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (oldpwd)
	{
		printf("%s\n", oldpwd);
		free(oldpwd);
	}
	else
	{
		perror("getcwd");
		g_exit_status = 1;
	}
}

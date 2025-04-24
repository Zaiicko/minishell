/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:29:18 by nicleena          #+#    #+#             */
/*   Updated: 2025/04/23 20:07:13 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_cd_slash(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
		printf("%s\n", cwd);
	else
		perror("getcwd");
	free(cwd);
}
void	ft_cd_empty(void)
{
	char	*home;

	home = getenv("HOME");
	if (home)
		ft_cd(home);
	else
		fprintf(stderr, "cd: HOME not set\n");
}
void	ft_cd_noargs(void)
{
	char *home;

	home = getenv("HOME");
	if (home)
		ft_cd(home);
	else
		fprintf(stderr, "cd: HOME not set\n");
}
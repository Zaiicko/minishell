/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:29:18 by nicleena          #+#    #+#             */
/*   Updated: 2025/04/24 15:00:35 by nicleena         ###   ########.fr       */
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

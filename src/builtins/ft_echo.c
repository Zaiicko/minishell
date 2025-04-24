/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:19:41 by nicleena          #+#    #+#             */
/*   Updated: 2025/04/23 20:07:22 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_echo(char **arg)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (arg[i] && ft_strncmp(arg[i], "-n", 2) == 0)
	{
		newline = 0;
		i++;
	}
	while (arg[i])
	{
		printf("%s", arg[i]);
		if (arg[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}

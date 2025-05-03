/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:50:40 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/03 17:18:14 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	is_numeric_arg(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	handle_exit(char **args)
{
	int	exit_code;

	ft_putstr_fd("exit\n", 1);
	if (args[1])
	{
		if (!is_numeric_arg(args[1]))
		{
			ft_putstr_error("minishell: exit: ", args[1],
				": numeric argument required");
			exit(255);
		}
		else if (args[2])
		{
			ft_putstr_error("minishell: exit: ", "too many arguments", NULL);
			g_exit_status = 1;
			return (g_exit_status);
		}
		exit_code = ft_atoi(args[1]) % 256;
		exit(exit_code);
	}
	exit(g_exit_status);
	return (0);
}

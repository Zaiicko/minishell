/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:19:41 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/02 15:42:06 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

#include "../../inc/minishell.h"

static int is_n_option(char *arg)
{
    int i;

    if (!arg || arg[0] != '-')
        return (0);
    
    i = 1;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return (0);
        i++;
    }
    if (i <= 1)
        return (0);   
    return (1);
}

void ft_echo(char **arg)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
    while (arg[i] && is_n_option(arg[i]))
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
    g_exit_status = 0;
}

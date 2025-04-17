/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:31:22 by nicleena          #+#    #+#             */
/*   Updated: 2025/04/17 15:32:36 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int is_builtin(char *cmd)
{
    return (
        !strcmp(cmd, "cd") ||
        !strcmp(cmd, "echo") ||
        !strcmp(cmd, "pwd") ||
        !strcmp(cmd, "env") ||
        !strcmp(cmd, "export") ||
        !strcmp(cmd, "unset") ||
        !strcmp(cmd, "exit")
    );
}

int exec_builtin(char **args, t_env *env)
{
    char *equal;
    
    if (!args || !args[0])
        return (0);
    if (!strcmp(args[0], "cd"))
        ft_cd(args[1] ? args[1] : getenv("HOME"));
    else if (!strcmp(args[0], "echo"))
        ft_echo(args);
    else if (!strcmp(args[0], "pwd"))
        ft_pwd();
    else if (!strcmp(args[0], "env"))
        ft_env(env);
    else if (!strcmp(args[0], "export") && args[1])
    {
        equal = strchr(args[1], '=');
        if (equal)
        {
            *equal = '\0';
            ft_setenv(env, args[1], equal + 1);
        }
    }
    else if (!strcmp(args[0], "unset") && args[1])
        ft_unset(env, args[1]);
    else if (!strcmp(args[0], "exit"))
        exit(0);
    else
        return (0);
    return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:31:22 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/02 14:29:18 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_builtin(char *cmd)
{
	return (!strcmp(cmd, "cd") || !strcmp(cmd, "echo") || !strcmp(cmd, "pwd")
		|| !strcmp(cmd, "env") || !strcmp(cmd, "export") || !strcmp(cmd,
			"unset") || !strcmp(cmd, "exit"));
}

void	handle_cd(char **args)
{
	char	*target;

	target = NULL;
	if (args[1])
		target = args[1];
	else
		target = getenv("HOME");
	ft_cd(target);
}

static int	exec_builtin_cd_export_unset(char **args, t_env *env)
{
	char	*equal;

	if (!strcmp(args[0], "cd"))
	{
		handle_cd(args);
		return (1);
	}
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
	else
		return (0);
	return (1);
}

int exec_builtin(char **args, t_env *env)
{
    if (!args || !args[0])
        return (0);
        
    if (!strcmp(args[0], "echo"))
    {
        ft_echo(args);
        return (0);
    }
    else if (!strcmp(args[0], "pwd"))
    {
        ft_pwd();
        return (g_exit_status);
    }
    else if (!strcmp(args[0], "env"))
    {
        ft_env(env);
        return (0);
    }
    else if (!strcmp(args[0], "exit"))
    {
        return (handle_exit(args));
    }
    else
        return (exec_builtin_cd_export_unset(args, env));
}
int handle_exit(char **args)
{
    int exit_code = g_exit_status;
    
    printf("exit\n");
    
    if (args[1])
    {
        char *endptr;
        long code = strtol(args[1], &endptr, 10);
        
        if (*endptr != '\0')
        {
            fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
            exit(255);
        }
        else if (args[2])
        {
            fprintf(stderr, "minishell: exit: too many arguments\n");
            return 1;
        }
        else
        {
            exit_code = code % 256;
        }
    }
    
    exit(exit_code);
}
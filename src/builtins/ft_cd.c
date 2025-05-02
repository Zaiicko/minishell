/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:19:44 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/02 14:51:51 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void update_pwd_vars(char *oldpwd)
{
    char *newpwd;

    newpwd = getcwd(NULL, 0);
    if (newpwd)
    {
        setenv("OLDPWD", oldpwd, 1);
        setenv("PWD", newpwd, 1);
        free(newpwd);
        g_exit_status = 0;
    }
    else
    {
        perror("getcwd");
        g_exit_status = 1;
    }
}

void ft_tilde(void)
{
    char *home;

    home = getenv("HOME");
    if (home)
    {
        char *oldpwd = getcwd(NULL, 0);
        if (chdir(home) == -1)
        {
            fprintf(stderr, "minishell: cd: %s: %s\n", home, strerror(errno));
            g_exit_status = 1;
        }
        else
        {
            update_pwd_vars(oldpwd);
        }
        free(oldpwd);
    }
    else
    {
        fprintf(stderr, "minishell: cd: HOME not set\n");
        g_exit_status = 1;
    }
}

void ft_cd_oldpwd(void)
{
    char *oldpwd;

    oldpwd = getenv("OLDPWD");
    if (oldpwd)
    {
        char *current = getcwd(NULL, 0);
        if (chdir(oldpwd) == -1)
        {
            fprintf(stderr, "minishell: cd: %s: %s\n", oldpwd, strerror(errno));
            g_exit_status = 1;
        }
        else
        {
            printf("%s\n", oldpwd);
            update_pwd_vars(current);
        }
        free(current);
    }
    else
    {
        fprintf(stderr, "minishell: cd: OLDPWD not set\n");
        g_exit_status = 1;
    }
}

int ft_cd(char **args)
{
    char *path;
    char *oldpwd;

    if (!args[1])
    {
        ft_tilde();
        return (g_exit_status);
    }
    path = args[1];
    if (ft_strncmp(path, "~", 1) == 0)
    {
        ft_tilde();
        return (g_exit_status);
    }
    else if (ft_strncmp(path, "-", 1) == 0)
    {
        ft_cd_oldpwd();
        return (g_exit_status);
    }
    else if (ft_strncmp(path, "/", 1) == 0 || ft_strncmp(path, "/", 1) == 0)
    {
        oldpwd = getcwd(NULL, 0);
        if (chdir(path) == -1)
        {
            fprintf(stderr, "minishell: cd: %s: %s\n", path, strerror(errno));
            g_exit_status = 1;
            free(oldpwd);
        }
        else
        {
            update_pwd_vars(oldpwd);
            free(oldpwd);
        }
        return (g_exit_status);
    }
    oldpwd = getcwd(NULL, 0);
    if (chdir(path) == -1)
    {
        fprintf(stderr, "minishell: cd: %s: %s\n", path, strerror(errno));
        g_exit_status = 1;
        free(oldpwd);
    }
    else
    {
        update_pwd_vars(oldpwd);
        free(oldpwd);
    }
    return (g_exit_status);
}

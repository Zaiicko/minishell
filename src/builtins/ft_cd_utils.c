/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:29:18 by nicleena          #+#    #+#             */
/*   Updated: 2025/04/14 16:29:51 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void ft_cd_slash(void)
{
    char *cwd;

    cwd = getcwd(NULL, 0);
    if (cwd)
        printf("%s\n", cwd);
    else
        perror("getcwd");
    free(cwd);
}
void ft_cd_empty(void)
{
    char *home;

    home = getenv("HOME");
    if (home)
        ft_cd(home);
    else
        fprintf(stderr, "cd: HOME not set\n");
}
void ft_cd_invalid(void)
{
    fprintf(stderr, "cd: invalid option\n");
    fprintf(stderr, "usage: cd [OPTION] [DIR]\n");
    fprintf(stderr, "Try 'cd --help' for more information.\n");
}
void ft_cd_help(void)
{
    fprintf(stderr, "usage: cd [OPTION] [DIR]\n");
    fprintf(stderr, "Change the current directory to DIR.\n");
    fprintf(stderr, "If DIR is not specified, the value of the HOME\n");
    fprintf(stderr, "environment variable is used as the default.\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -L\tuse the logical directory structure\n");
    fprintf(stderr, "  -P\tuse the physical directory structure\n");
}
void ft_cd_noargs(void)
{
    char *home;

    home = getenv("HOME");
    if (home)
        ft_cd(home);
    else
        fprintf(stderr, "cd: HOME not set\n");
}
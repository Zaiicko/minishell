/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 22:37:38 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/03 16:16:06 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exit_perror(char *msg)
{
	perror(msg);
	exit(1);
}

void	free_all(t_data *data)
{
	if (data->input)
		free(data->input);
	if (data->tokens)
		free_token_list(&data->tokens);
	if (data->ast)
		free_ast(data->ast);
	if (data->env)
		free_env(data->env);
}

void	free_all_and_exit_perror(t_data *data, char *msg)
{
	free_all(data);
	exit_perror(msg);
}

void	ft_putstr_error(char *s1, char *s2, char *s3)
{
	if (s1)
		write(STDERR_FILENO, s1, ft_strlen(s1));
	if (s2)
		write(STDERR_FILENO, s2, ft_strlen(s2));
	if (s3)
		write(STDERR_FILENO, s3, ft_strlen(s3));
	write(STDERR_FILENO, "\n", 1);
}

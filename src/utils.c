/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 22:37:38 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/06 18:07:37 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

void	free_all_and_exit(t_data *data, char *msg)
{
	free_all(data);
	printf("%s", msg);
	exit(1);
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

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (0);
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

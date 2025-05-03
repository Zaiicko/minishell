/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:33:41 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/03 18:35:11 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_env(t_env *env)
{
	t_env_var	*current;
	t_env_var	*next;

	if (!env)
		return ;
	current = env->head;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	free(env);
}

void	ft_env(t_env *env)
{
	t_env_var	*current;

	if (!env)
		return ;
	current = env->head;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	g_exit_status = 0;
}

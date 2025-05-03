/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:19:20 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/03 16:23:10 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	unset_one_var(t_env *env, char *key)
{
	t_env_var	*current;

	if (!env || !env->head || !key)
		return ;
	current = env->head;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
		{
			if (current->prev)
				current->prev->next = current->next;
			else
				env->head = current->next;
			if (current->next)
				current->next->prev = current->prev;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		current = current->next;
	}
}

static int	is_valid_identifier(char *key)
{
	int	i;

	if (!key || !*key)
		return (0);
	if (!isalpha(key[0]) && key[0] != '_')
		return (0);
	i = 1;
	while (key[i])
	{
		if (!isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_unset(char **args, t_env *env)
{
	int	i;
	int	status;

	status = 0;
	if (!args || !args[1])
	{
		g_exit_status = 0;
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_error("minishell: unset: `", args[i],
				"': not a valid identifier");
			status = 1;
		}
		else
			unset_one_var(env, args[i]);
		i++;
	}
	g_exit_status = status;
	return (status);
}

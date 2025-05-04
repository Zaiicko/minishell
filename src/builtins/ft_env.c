/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:31:30 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/05 01:22:01 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_env_var	*create_env_var(char *key, char *value, t_data *data)
{
	t_env_var	*new_var;

	new_var = malloc(sizeof(t_env_var));
	if (!new_var)
		free_all_and_exit_perror(data, "Error\n Malloc allocation failed\n");
	new_var->key = strdup(key);
	if (!new_var->key)
	{
		free(new_var);
		free_all_and_exit_perror(data, "Error\n Malloc allocation failed\n");
	}
	new_var->value = strdup(value);
	if (!new_var->value)
	{
		free(new_var->key);
		free(new_var);
		free_all_and_exit_perror(data, "Error\n Malloc allocation failed\n");
	}
	return (new_var);
}

static void	update_existing_env(t_env_var *current, char *value, t_data *data)
{
	if (!value)
		return ;
	free(current->value);
	current->value = ft_strdup(value);
	if (!current->value)
		free_all_and_exit_perror(data, "Error\n Malloc failed\n");
}

void	ft_setenv(t_env *env, char *key, char *value, t_data *data)
{
	t_env_var	*current;
	t_env_var	*new_var;

	if (!env || !key)
		return ;
	current = env->head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			update_existing_env(current, value, data);
			return ;
		}
		current = current->next;
	}
	new_var = create_env_var(key, value, data);
	new_var->next = env->head;
	new_var->prev = NULL;
	if (env->head)
		env->head->prev = new_var;
	env->head = new_var;
}

static void	process_env_entry(t_env *env, char *entry, t_data *data)
{
	char	*key;
	char	*value;
	char	*equal_sign;

	equal_sign = strchr(entry, '=');
	if (!equal_sign)
		return ;
	key = ft_substr(entry, 0, equal_sign - entry);
	if (!key)
		free_all_and_exit_perror(data, "Error\n Malloc failed\n");
	value = ft_strdup(equal_sign + 1);
	if (!value)
	{
		free(key);
		free_all_and_exit_perror(data, "Error\n Malloc failed\n");
	}
	ft_setenv(env, key, value, data);
	free(key);
	free(value);
}

t_env	*init_env(char **envp, t_data *data)
{
	t_env	*env;
	int		i;

	env = malloc(sizeof(t_env));
	if (!env)
		free_all_and_exit_perror(data, "Error\n Malloc failed\n");
	env->head = NULL;
	env->next = NULL;
	i = 0;
	while (envp[i])
	{
		process_env_entry(env, envp[i], data);
		i++;
	}
	return (env);
}

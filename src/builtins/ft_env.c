/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:31:30 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/03 16:22:03 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_env(t_env *env)
{
	t_env_var	*current;

	if (!env || !env->head)
		return ;
	current = env->head;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		else
			printf("%s\n", current->key);
		current = current->next;
	}
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
		if (strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = strdup(value);
			if (!current->value)
				free_all_and_exit_perror(data,
					"Error\n Malloc allocation failed\n");
			return ;
		}
		current = current->next;
	}
	new_var = malloc(sizeof(t_env_var));
	if (!new_var)
	{
		free(value);
		free(key);
		free(env);
		free_all_and_exit_perror(data, "Error\n Malloc allocation failed\n");
	}
	new_var->key = strdup(key);
	if (!new_var->key)
	{
		free(value);
		free(key);
		free(env);
		free(new_var);
		free_all_and_exit_perror(data, "Error\n Malloc allocation failed\n");
	}
	new_var->value = strdup(value);
	if (!new_var->value)
	{
		free(new_var->key);
		free(value);
		free(key);
		free(env);
		free(new_var);
		free_all_and_exit_perror(data, "Error\n Malloc allocation failed\n");
	}
	new_var->next = env->head;
	new_var->prev = NULL;
	if (env->head)
		env->head->prev = new_var;
	env->head = new_var;
}

void	ft_unsetenv(t_env *env, char *key)
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

t_env	*init_env(char **envp, t_data *data)
{
	t_env	*env;
	int		i;
	char	*key;
	char	*value;
	char	*equal_sign;

	env = malloc(sizeof(t_env));
	if (!env)
		free_all_and_exit_perror(data, "Error\n Malloc allocation failed\n");
	env->head = NULL;
	env->next = NULL;
	i = 0;
	while (envp[i])
	{
		equal_sign = strchr(envp[i], '=');
		if (equal_sign)
		{
			key = ft_substr(envp[i], 0, equal_sign - envp[i]);
			if (!key)
			{
				free(env);
				free_all_and_exit_perror(data,
					"Error\n Malloc allocation failed\n");
			}
			value = ft_strdup(equal_sign + 1);
			if (!value)
			{
				free(env);
				free(key);
				free_all_and_exit_perror(data,
					"Error\n Malloc allocation failed\n");
			}
			ft_setenv(env, key, value, data);
			free(key);
			free(value);
		}
		i++;
	}
	return (env);
}

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
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	free(env);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:31:37 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/07 17:02:36 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	handle_export_arg(t_env *env, char *arg, t_data *data)
{
	char	*equal_sign;
	char	*name;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		name = arg;
		value = equal_sign + 1;
		if (!is_valid_identifier(name))
		{
			ft_putstr_error("minishell: export: `", arg,
				"': not a valid identifier\n");
			*equal_sign = '=';
			return (1);
		}
		ft_setenv(env, name, value, data);
		*equal_sign = '=';
		return (0);
	}
	else if (!is_valid_identifier(arg))
		return (ft_putstr_error("minishell: export: `", arg,
				"': not a valid identifier\n"), 1);
	return (0);
}

int	ft_export_with_args(t_env *env, char **args, t_data *data)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (args[i])
	{
		if (handle_export_arg(env, args[i], data) != 0)
			ret = 1;
		i++;
	}
	g_exit_status = ret;
	return (ret);
}

void	ft_export(t_env *env)
{
	t_env_var	*current;

	if (!env || !env->head)
		return ;
	current = env->head;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
	g_exit_status = 0;
}

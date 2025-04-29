/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 22:46:07 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/29 20:55:49 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*extract_var_name(char *str, int *index_ptr)
{
	int		start_pos;
	char	*var_name;

	(*index_ptr)++;
	if (str[*index_ptr] == '?')
	{
		(*index_ptr)++;
		var_name = ft_strdup("?");
		if (!var_name)
			return (NULL);
		return (var_name);
	}
	start_pos = *index_ptr;
	while (str[*index_ptr] && (ft_isalnum(str[*index_ptr])
			|| str[*index_ptr] == '_'))
		(*index_ptr)++;
	var_name = ft_substr(str, start_pos, *index_ptr - start_pos);
	if (!var_name)
		return (NULL);
	return (var_name);
}

char	*get_env_value_helper(t_data *data, t_env *env, char *var_name)
{
	t_env_var	*current_var;
	char		*value_str;

	current_var = env->head;
	while (current_var)
	{
		if (ft_strncmp(current_var->key, var_name, ft_strlen(current_var->key)) == 0)
		{
			if (current_var->value)
				value_str = ft_strdup(current_var->value);
			else
				value_str = ft_strdup("");
			if (!value_str)
				free_all_and_exit_perror(data, "Error\n Malloc failed\n");
			return (value_str);
		}
		current_var = current_var->next;
	}
	value_str = ft_strdup("");
	if (!value_str)
		free_all_and_exit_perror(data, "Error\n Malloc failed\n");
	return (value_str);
}

char	*get_env_value(t_data *data, t_env *env, char *var_name)
{
	char		*value_str;
	extern int	g_exit_status;

	if (!var_name)
		free_all_and_exit_perror(data, "Error\n NULL variable name\n");
	if (ft_strncmp(var_name, "?", ft_strlen(var_name)) == 0)
	{
		value_str = ft_itoa(g_exit_status);
		if (!value_str)
			free_all_and_exit_perror(data, "Error\n Malloc failed\n");
		return (value_str);
	}
	return (get_env_value_helper(data, env, var_name));
}

void	handle_squote(char *str, char *result, t_expander *exp)
{
	exp->in_squotes = !exp->in_squotes;
	result[exp->write_index++] = str[exp->read_index++];
}

void	handle_dquote(char *str, char *result, t_expander *exp)
{
	exp->in_dquotes = !exp->in_dquotes;
	result[exp->write_index++] = str[exp->read_index++];
}

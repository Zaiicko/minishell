/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:07:02 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/30 19:51:37 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	process_var_len(t_data *data, char *str, int *index_ptr,
	int *length_ptr)
{
	int		temp_idx;
	char	*var_name;
	char	*var_value;

	temp_idx = *index_ptr;
	var_name = extract_var_name(str, &temp_idx);
	if (!var_name)
		return ;
	var_value = get_env_value(data->env, var_name);
	if (var_value)
		*length_ptr += ft_strlen(var_value);
	*index_ptr = temp_idx - 1;
	free(var_name);
	free(var_value);
}

int	calculate_expanded_length(t_data *data, char *str)
{
	int	index;
	int	result_len;
	t_expander	exp;

	if (!str)
		return (0);
	index = 0;
	result_len = 0;
	exp.in_squotes = 0;
	exp.in_dquotes = 0;

	while (str[index])
	{
		if (str[index] == '\'' && !exp.in_dquotes)
		{
			exp.in_squotes = !exp.in_squotes;
			result_len++;
		}
		else if (str[index] == '\"' && !exp.in_squotes)
		{
			exp.in_dquotes = !exp.in_dquotes;
			result_len++;
		}
		else if (str[index] == '$' && !exp.in_squotes
			&& str[index + 1] && ((str[index + 1] != '\"'
					&& str[index + 1] != '\'') || exp.in_dquotes)
			&& (ft_isalnum(str[index + 1]) || str[index + 1] == '_'
				|| str[index + 1] == '?'))
		{
			exp.read_index = index;
			process_var_len(data, str, &index, &result_len);
		}
		else
			result_len++;
		index++;
	}
	return (result_len + 1);
}

int	is_expandable_var(char *str, t_expander *exp)
{
	return (str[exp->read_index] == '$' && !exp->in_squotes
		&& str[exp->read_index + 1]
		&& ((str[exp->read_index + 1] != '\"'
				&& str[exp->read_index + 1] != '\'') || exp->in_dquotes)
		&& (ft_isalnum(str[exp->read_index + 1])
			|| str[exp->read_index + 1] == '_'
			|| str[exp->read_index + 1] == '?'));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 19:46:16 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/29 21:43:51 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_variables(t_data *data, char *str)
{
	t_expander	exp;
	char		*result;
	int			max_size;

	if (!str)
		return (NULL);
	max_size = calculate_expanded_length(data, str);
	result = ft_calloc(max_size, sizeof(char));
	if (!result)
		return (NULL);
	exp.read_index = 0;
	exp.write_index = 0;
	exp.in_squotes = 0;
	exp.in_dquotes = 0;
	while (str[exp.read_index] && exp.write_index < max_size - 1)
	{
		if (str[exp.read_index] == '\'' && !exp.in_dquotes)
			handle_squote(str, result, &exp);
		else if (str[exp.read_index] == '\"' && !exp.in_squotes)
			handle_dquote(str, result, &exp);
		else if (is_expandable_var(str, &exp))
			handle_variable_expansion(data, str, result, &exp);
		else
			result[exp.write_index++] = str[exp.read_index++];
	}
	result[exp.write_index] = '\0';
	return (result);
}

void	handle_quotes(char *str, char *result, t_expander *exp)
{
	exp->in_squotes = !exp->in_squotes;
	result[exp->write_index++] = str[exp->read_index++];
}

void	handle_variable_expansion(t_data *data, char *str, char *result,
	t_expander *exp)
{
	char	*var_name;
	char	*var_value;
	int		i;

	var_name = extract_var_name(str, &exp->read_index);
	if (!var_name)
	{
		free(result);
		free_all_and_exit_perror(data,
			"Error\n Exctract the variable name failed\n");
	}
	var_value = get_env_value(data->env, var_name);
	if (!var_value)
	{
		free(result);
		free(var_name);
		free_all_and_exit_perror(data,
			"Error\n Exctract the variable value failed\n");
	}
	i = 0;
	while (var_value && var_value[i])
		result[exp->write_index++] = var_value[i++];
	free(var_name);
	free(var_value);
}

void	expand_ast_redirections(t_data *data, t_ast_node *node)
{
	char	*expanded;

	if (!node || !node->redir_file)
		return ;
	expanded = expand_variables(data, node->redir_file);
	if (expanded)
	{
		free(node->redir_file);
		node->redir_file = expanded;
	}
	else
		free_all_and_exit_perror(data,
			"Error\n Variable expansion failed\n");
}

void	expand_ast(t_data *data, t_ast_node *node)
{
	int		arg_idx;
	char	*expanded;

	if (!node)
		return ;
	if (node->type == NODE_COMMAND && node->args)
	{
		arg_idx = 0;
		while (node->args[arg_idx])
		{
			expanded = expand_variables(data, node->args[arg_idx]);
			if (expanded)
			{
				free(node->args[arg_idx]);
				node->args[arg_idx] = expanded;
			}
			else
				free_all_and_exit_perror(data,
					"Error\n Variable expansion failed\n");
			arg_idx++;
		}
	}
	expand_ast_redirections(data, node);
	expand_ast(data, node->l);
	expand_ast(data, node->r);
}

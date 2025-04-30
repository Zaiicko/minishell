/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:06:12 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/01 01:34:33 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	process_quotes_in_ast(t_data *data, t_ast_node *node)
{
	if (!node)
		return ;
	prepare_cmd_for_execution(data, node);
	process_quotes_in_ast(data, node->l);
	process_quotes_in_ast(data, node->r);
}

static char	*strip_quotes(t_data *data, char *str)
{
	int		i;
	int		j;
	char	*result;
	char	quote_type;

	if (!str)
		return (NULL);
	result = (char *)malloc(sizeof(char) * ft_strlen(str) + 1);
	if (!result)
		free_all_and_exit_perror(data, "Error\n Malloc allocation failed\n");
	i = 0;
	j = 0;
	quote_type = 0;
	while (str[i])
	{
		if (ft_isquote(str[i]))
		{
			if (quote_type == 0)
				quote_type = str[i];
			else if (quote_type == str[i])
				quote_type = 0;
			else
				result[j++] = str[i];
		}
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	*remove_quotes(t_data *data, char *str)
{
	char	*result;

	if (!str)
		return (NULL);
	result = strip_quotes(data, str);
	free(str);
	return (result);
}

void	clean_quotes_from_args(t_data *data, char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		args[i] = remove_quotes(data, args[i]);
		i++;
	}
}

void	prepare_cmd_for_execution(t_data *data, t_ast_node *node)
{
	char	*expanded;

	if (!node)
		return ;
	if (node->type == NODE_COMMAND && node->args)
		clean_quotes_from_args(data, node->args);
	if (node->redir_file)
	{
		expanded = remove_quotes(data, ft_strdup(node->redir_file));
		if (expanded)
		{
			free(node->redir_file);
			node->redir_file = expanded;
		}
	}
}

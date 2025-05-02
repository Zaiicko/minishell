/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:06:12 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/02 15:05:50 by nicleena         ###   ########.fr       */
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

static int ft_isquote(char c)
{
    return (c == '\'' || c == '"');
}

static void	fill_stripped_result(char *str, char *result)
{
	int		i;
	int		j;
	char	quote_type;

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
}

static char	*strip_quotes(t_data *data, char *str)
{
	char	*result;

	if (!str)
		return (NULL);
	result = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!result)
		free_all_and_exit_perror(data, "Error\n Malloc allocation failed\n");
	fill_stripped_result(str, result);
	return (result);
}

void	clean_quotes_from_args(t_data *data, char **args)
{
	int		i;
	char	*result;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		result = strip_quotes(data, args[i]);
		free(args[i]);
		args[i] = result;
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
		expanded = strip_quotes(data, node->redir_file);
		free(node->redir_file);
		node->redir_file = expanded;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 11:48:47 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/07 00:06:24 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	fill_command_args(t_data *data, t_token **tokens, char **args,
		int count)
{
	int		i;
	t_token	*current;
	t_token	*next;

	i = 0;
	current = *tokens;
	while (current && i < count)
	{
		next = process_token(data, current, args, &i);
		if (!next)
			break ;
		current = next;
	}
	args[i] = NULL;
	*tokens = current;
}

t_ast_node	*parse_command(t_data *data, t_token **tokens)
{
	t_ast_node	*cmd_node;
	char		**args;
	int			count;

	if ((*tokens) && is_redirection_token((*tokens)->type))
		return (parse_firstredirect_command(data, tokens));
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
		return (NULL);
	count = count_command_args(*tokens);
	args = (char **)malloc((count + 1) * sizeof(char *));
	if (!args)
		free_all_and_exit(data, "Error\n Malloc failed\n");
	fill_command_args(data, tokens, args, count);
	cmd_node = new_command_node(args);
	if (!cmd_node)
	{
		ft_free_tab(args);
		free_all_and_exit(data, "Error\n Node creation failed\n");
	}
	return (handle_redirections(data, tokens, cmd_node));
}

t_ast_node	*parse_pipe(t_data *data, t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*node;

	left = parse_command(data, tokens);
	if (!left)
		return (NULL);
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		if (!*tokens || (*tokens)->type == TOKEN_PIPE)
		{
			ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
			ft_putstr_fd("near unexpected token `|'\n", STDERR_FILENO);
			g_exit_status = 258;
			return (free_ast(left), NULL);
		}
		node = new_pipe_node(left, parse_pipe(data, tokens));
		if (!node)
		{
			free_ast(left);
			free_all_and_exit(data, "Error\n Node creation failed\n");
		}
		return (node);
	}
	return (left);
}

t_ast_node	*parse(t_data *data)
{
	t_token		*current;
	t_ast_node	*root;

	if (!data->tokens)
		return (NULL);
	current = data->tokens;
	if ((*current).type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `|'\n", STDERR_FILENO);
		g_exit_status = 258;
		return (NULL);
	}
	root = parse_pipe(data, &current);
	if (!root)
		return (NULL);
	return (root);
}

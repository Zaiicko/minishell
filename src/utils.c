/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 22:37:38 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/20 13:34:44 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	exit_perror(char *msg)
{
	perror(msg);
	exit(1);
}

void	free_all(t_token **tokens, t_ast_node **ast)
{
	free_token_list(tokens);
	free_ast(*ast);
}

void	free_token_list_and_exit_perror(t_token **tokens, char *msg)
{
	free_token_list(tokens);
	exit_perror(msg);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 03:44:00 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/01 03:49:21 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_quotes(char *str, char *result, t_expander *exp)
{
	exp->in_squotes = !exp->in_squotes;
	result[exp->write_index++] = str[exp->read_index++];
}

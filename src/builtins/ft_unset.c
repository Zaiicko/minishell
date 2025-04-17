/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:19:20 by nicleena          #+#    #+#             */
/*   Updated: 2025/04/17 15:28:47 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void ft_unset(t_env *env, char *key)
{
    t_env_var *current;

    if (!env || !env->head || !key)
        return;

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
            return;
        }
        current = current->next;
    }
}

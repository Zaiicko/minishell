/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 21:03:27 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/05 21:04:43 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**ft_tabdup(char **tab)
{
	char	**dup;
	int		i;

	if (!tab)
		return (NULL);
	i = 0;
	while (tab[i])
		i++;
	dup = (char **)malloc(sizeof(char *) * (i + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (tab[i])
	{
		dup[i] = ft_strdup(tab[i]);
		if (!dup[i])
		{
			ft_free_tab(dup);
			return (NULL);
		}
		i++;
	}
	dup[i] = NULL;
	return (dup);
}
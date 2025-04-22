/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:29:20 by nicleena          #+#    #+#             */
/*   Updated: 2025/04/17 15:29:38 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*prev;
	struct s_env_var	*next;
}						t_env_var;

typedef struct s_env
{
	t_env_var			*head;
	t_env_var			*next;
}						t_env;

#endif
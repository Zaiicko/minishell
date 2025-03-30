/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:14:40 by nicleena          #+#    #+#             */
/*   Updated: 2025/03/30 22:47:16 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <termios.h>

// typedef struct t_data;

// struct s_data
// {
//     int id;    
// };


void	readline_loop(void);
void	init_signals(void);
void	handle_sigint(int sig);
void	config_terminal(void);

#endif

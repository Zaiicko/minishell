/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:53:58 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/03 18:29:42 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int g_exit_status = 0;

void	load_history(void)
{
	int	fd;
	char	*line;
	int	len;

	fd = open(".readline_history", O_RDONLY);
	if (fd < 0)
	{
		if(errno != ENOENT)
			exit_perror("Error\n Can't open history file\n");
		return;
	}
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break;
		len = strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (line[0])
			add_history(line);
		free(line);
	}
	close(fd);
}

void	save_history(char *line)
{
	int	fd;

	if (!line || !line[0])
		return;
	fd = open(".readline_history", O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
		exit_perror("Error\n Can't open history file\n");
	write(fd, line, strlen(line));
	write(fd, "\n", 1);
	close(fd);
}

void	readline_loop(void)
{
	char *input;

	load_history();
	start_signals();
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break;	
		}
		if (input[0])
		{
			add_history(input);
			save_history(input);
		}
		free(input);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 00:15:17 by zaiicko           #+#    #+#             */
/*   Updated: 2025/04/21 23:53:55 by zaiicko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	load_history(void)
{
	int		fd;
	char	*line;
	int		len;

	fd = open(".readline_history", O_RDONLY);
	if (fd < 0)
	{
		if (errno != ENOENT)
			exit_perror("Error\n Can't open history file\n");
		return ;
	}
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		len = strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (line[0])
			add_history(line);
		free(line);
	}
	close(fd);
}

void	save_history(t_data *data)
{
	int	fd;

	if (!data->input || !data->input[0])
		return ;
	fd = open(".readline_history", O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
		free_all_and_exit_perror(data, "Error\n Can't open history file\n");
	write(fd, data->input, strlen(data->input));
	write(fd, "\n", 1);
	close(fd);
}

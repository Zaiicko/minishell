/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 00:15:17 by zaiicko           #+#    #+#             */
/*   Updated: 2025/05/03 18:31:44 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*get_history_path(void)
{
	char	*home_dir;
	char	*history_path;

	home_dir = getenv("HOME");
	if (!home_dir)
		return (NULL);
	history_path = ft_strjoin(home_dir, "/.minishell_history");
	if (!history_path)
		exit_perror("Error\n Memory allocation failed\n");
	return (history_path);
}

static void	process_history_line(char *line)
{
	int	len;

	len = strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	if (line[0])
		add_history(line);
}

void	load_history(void)
{
	int		fd;
	char	*line;
	char	*history_path;

	history_path = get_history_path();
	if (!history_path)
		return ;
	fd = open(history_path, O_RDONLY);
	free(history_path);
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
		process_history_line(line);
		free(line);
	}
	close(fd);
}

void	save_history(t_data *data)
{
	int		fd;
	char	*history_path;

	if (!data->input || !data->input[0])
		return ;
	history_path = get_history_path();
	if (!history_path)
		return ;
	fd = open(history_path, O_WRONLY | O_APPEND | O_CREAT, 0644);
	free(history_path);
	if (fd < 0)
		free_all_and_exit_perror(data, "Error\n Can't open history file\n");
	write(fd, data->input, strlen(data->input));
	write(fd, "\n", 1);
	close(fd);
}

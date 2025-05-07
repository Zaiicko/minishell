/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:43:10 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/07 17:56:37 by nicleena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*create_temp_file(void)
{
	static int	index = 0;
	char		*temp_name;
	char		*index_str;
	char		*tmp;

	index_str = ft_itoa(index++);
	if (!index_str)
		return (NULL);
	tmp = ft_strjoin("/tmp/minishell_heredoc_", index_str);
	free(index_str);
	if (!tmp)
		return (NULL);
	temp_name = ft_strdup(tmp);
	free(tmp);
	return (temp_name);
}

static int	write_heredoc_content(int fd, char *delimiter)
{
	char	*line;
	int		result;

	result = 0;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_error("minishell: warning: ",
				"here-document delimited by end-of-file\n", NULL);
			break ;
		}
		if (g_exit_status == 130)
			return (free(line), 0);
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			result = 1;
			break ;
		}
		result = 1;
		return (ft_putendl_fd(line, fd), free(line), result);
	}
	return (result);
}

char	*handle_heredoc(char *delimiter, t_data *data)
{
	char	*temp_file;
	int		fd;
	int		result;

	temp_file = create_temp_file();
	if (!temp_file)
		free_all_and_exit(data, "Error\n Malloc failed\n");
	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return (free(temp_file), NULL);
	result = write_heredoc_content(fd, delimiter);
	close(fd);
	if (!result)
	{
		unlink(temp_file);
		free(temp_file);
		return (NULL);
	}
	return (temp_file);
}

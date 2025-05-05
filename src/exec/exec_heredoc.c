/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicleena <nicleena@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:43:10 by nicleena          #+#    #+#             */
/*   Updated: 2025/05/05 15:41:08 by nicleena         ###   ########.fr       */
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

static void	prompt_heredoc_line(void)
{
    write(STDERR_FILENO, "> ", 2);
}

static int	write_heredoc_content(int fd, char *delimiter)
{
    char	*line;
    int		result;

    result = 0;
    while (1)
    {
        prompt_heredoc_line();
        line = readline("");
        if (!line)
        {
            ft_putstr_error("minishell: warning: ", 
                "here-document delimited by end-of-file\n", NULL);
            break;
        }
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
        result = 1;
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
        free_all_and_exit_perror(data, "Error\n Malloc failed\n");
    fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd < 0)
    {
        free(temp_file);
        free_all_and_exit_perror(data, "Error\n Cannot create heredoc file\n");
    }
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

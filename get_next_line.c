/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 21:12:00 by baelgadi          #+#    #+#             */
/*   Updated: 2025/06/02 23:49:56 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_fdnode	*get_fdnode(t_fdnode **list, int fd)
{
	t_fdnode	*node;

	node = *list;
	while (node)
	{
		if (node->fd == fd)
			return (node);
		node = node->next;
	}
	node = malloc(sizeof(t_fdnode));
	if (!node)
		return (NULL);
	node->fd = fd;
	node->stash = NULL;
	node->next = *list;
	*list = node;
	return (node);
}

static char	*extract_line(char *stash)
{
	int		len;
	char	*line;

	if (!stash || !stash[0])
		return (NULL);
	len = 0;
	while (stash[len] && stash[len] != '\n')
		len++;
	if (stash[len] == '\n')
		len++;
	line = ftg_substr(stash, 0, len);
	return (line);
}

static char	*update_stash(char *stash)
{
	size_t	len;
	char	*new_stash;
	size_t	initial_len;

	if (!stash)
		return (NULL);
	initial_len = ftg_strlen(stash);
	len = 0;
	while (stash[len] && stash[len] != '\n')
		len++;
	if (stash[len] == '\n')
		len++;
	if (len >= initial_len)
		return (free(stash), NULL);
	new_stash = ftg_substr(stash, len, ftg_strlen(stash + len));
	if (!new_stash)
		return (free(stash), NULL);
	free(stash);
	return (new_stash);
}

static char	*read_file(int fd, char *stash)
{
	char	*buf;
	int		bytes_read;
	char	*tmp;

	if (BUFFER_SIZE < 0)
		return (free(stash), NULL);
	buf = malloc((size_t)BUFFER_SIZE + 1);
	if (!buf)
		return (free(stash), NULL);
	bytes_read = 1;
	while (bytes_read > 0 && (!stash || !ftg_strchr(stash, '\n')))
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(buf), free(stash), NULL);
		buf[bytes_read] = '\0';
		tmp = ftg_strjoin(stash, buf);
		if (!tmp)
			return (free(buf), NULL);
		stash = tmp;
	}
	free(buf);
	return (stash);
}

char	*get_next_line(int fd)
{
	static t_fdnode	*fd_list = NULL;
	t_fdnode		*node;
	char			*line;
	char			*tmp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	node = get_fdnode(&fd_list, fd);
	if (!node)
		return (NULL);
	node->stash = read_file(fd, node->stash);
	if (!node->stash)
		return (free_fdnode(&fd_list, fd), NULL);
	line = extract_line(node->stash);
	if (!line)
		return (free_fdnode(&fd_list, fd), NULL);
	tmp = update_stash(node->stash);
	node->stash = tmp;
	if (!tmp)
		free_fdnode(&fd_list, fd);
	return (line);
}

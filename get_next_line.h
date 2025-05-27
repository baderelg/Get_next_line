/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 21:37:25 by baelgadi          #+#    #+#             */
/*   Updated: 2025/06/02 23:49:58 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_fdnode
{
	int				fd;
	char			*stash;
	struct s_fdnode	*next;
}	t_fdnode;

char	*get_next_line(int fd);
size_t	ftg_strlen(const char *s);
char	*ftg_strchr(const char *str, int c);
char	*ftg_strjoin(char *s1, char *s2);
char	*ftg_substr(char *s, unsigned int start, size_t len);
void	free_fdnode(t_fdnode **list, int fd);

#endif

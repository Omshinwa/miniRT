/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasamuel <dasamuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:16:59 by dasamuel          #+#    #+#             */
/*   Updated: 2026/03/16 07:44:18 by dasamuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../common.h"

char	*get_next_line_strip_nl(int fd)
{
	char	line_buf[4096];
	int		i;
	char	c;

	i = 0;
	while (i < 4095 && read(fd, &c, 1) == 1)
	{
		line_buf[i++] = c;
		if (c == '\n')
			break ;
	}
	if (i == 0)
		return (NULL);
	line_buf[i] = '\0';
	return (ft_strtrim(line_buf, "\n"));
}

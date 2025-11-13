/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:20:21 by vdiez-cu          #+#    #+#             */
/*   Updated: 2025/11/13 16:29:09 by vdiez-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <unistd.h>
# include <stdio.h>
# include <limits.h>
# include <errno.h>
# include <fcntl.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>
# include "libft/libft.h"

typedef struct s_game
{
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
	int		has_no;
	int		has_so;
	int		has_we;
	int		has_ea;
	int		floor[3];
	int		ceiling[3];
	int		has_floor;
	int		has_ceiling;
	int		map_height;
	int		map_width;
	char	**map;
}			t_game;

#endif
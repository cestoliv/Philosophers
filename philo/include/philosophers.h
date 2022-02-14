/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 12:24:34 by ocartier          #+#    #+#             */
/*   Updated: 2022/02/14 17:05:42 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include "../libft/include/libft.h"

typedef struct s_fork
{
	int		state;
	t_phil	*used_by;
}	t_fork;

typedef struct s_phil
{
	int		last_meal;
	t_fork	*r_fork;
	t_fork	*l_fork;
}	t_phil;
#endif

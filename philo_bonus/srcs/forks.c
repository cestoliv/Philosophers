/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 10:37:44 by ocartier          #+#    #+#             */
/*   Updated: 2022/03/05 12:29:54 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	take_fork(t_phil *phil)
{
	if (!is_dead(phil))
	{
		sem_wait(phil->params->sem_forks);
		write_state("has taken a fork", phil);
	}
}

void	release_forks_and_sleep(t_phil *phil)
{
	sem_post(phil->params->sem_forks);
	sem_post(phil->params->sem_forks);
	write_state("is sleeping", phil);
	ft_usleep(phil->params->time_to_sleep);
}

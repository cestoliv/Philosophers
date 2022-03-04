/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 10:37:44 by ocartier          #+#    #+#             */
/*   Updated: 2022/03/04 08:46:15 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	take_fork(char fork_name, t_phil *phil)
{
	int		*taken;

	if (!is_dead(phil))
	{
		taken = &(phil->r_taken);
		if (fork_name == 'l')
			taken = &(phil->l_taken);
		if (!(*taken))
		{
			sem_wait(phil->params->sem_forks);
			*taken = 1;
			write_state("has taken a fork", phil);
		}
	}
}

void	release_fork(char fork_name, t_phil *phil)
{
	int		*taken;

	taken = &(phil->r_taken);
	if (fork_name == 'l')
		taken = &(phil->l_taken);
	sem_post(phil->params->sem_forks);
	*taken = 0;
}

void	take_forks(t_phil *phil)
{
	sem_wait(phil->params->sem_forks);
	write_state("has taken a fork", phil);
	sem_wait(phil->params->sem_forks);
	write_state("has taken a fork", phil);
}

void	release_forks_and_sleep(t_phil *phil)
{
	release_fork('r', phil);
	release_fork('l', phil);
	//sem_post(phil->params->sem_forks);
	//sem_post(phil->params->sem_forks);
	write_state("is sleeping", phil);
	ft_usleep(phil->params->time_to_sleep);
}

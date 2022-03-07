/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 10:36:33 by ocartier          #+#    #+#             */
/*   Updated: 2022/03/07 16:52:15 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	stop_threads(t_phil **philos, t_params *params)
{
	int	cur;

	cur = 0;
	while (cur < params->num)
		kill((*philos)[cur++].pid, SIGTERM);
	return (EXIT_FAILURE);
}

int	check_philo_death(t_phil *phil, long cur_time)
{
	int	dead;
	int	last_meal;

	dead = 0;
	sem_wait(phil->sem_last_meal);
	last_meal = cur_time - phil->last_meal;
	sem_post(phil->sem_last_meal);
	if (last_meal > phil->params->time_to_die)
	{
		sem_wait(phil->params->sem_console);
		printf("%09ld %d died\n", cur_time, phil->pos);
		dead = 1;
	}
	return (dead);
}

void	*check_philos_death(void *arg)
{
	t_phil		**philos;
	t_params	*params;

	philos = (t_phil **)arg;
	params = philos[0]->params;

	sem_wait(params->finished);
	printf("thee end");
	stop_threads(philos, params);
	/*
	t_params	*params;
	t_phil		**philos;
	long		cur_time;
	int			cur;

	philos = (t_phil **)arg;
	params = philos[0]->params;
	while (1)
	{
		cur = 0;
		cur_time = get_timestamp() - params->start_time;
		while (cur < params->num)
		{
			if (check_philo_death(&(*philos)[cur], cur_time))
			{
				stop_threads(philos, params);
				return (NULL);
			}
			cur++;
		}
		ft_usleep(1);
	}
	*/
	return (NULL);
}

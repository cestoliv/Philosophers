/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 12:24:42 by ocartier          #+#    #+#             */
/*   Updated: 2022/03/02 10:14:58 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	*philo_life(void *arg)
{
	t_phil		*phil;

	phil = (t_phil *)arg;
	if (phil->pos % 2 != 0)
		ft_usleep(phil->params->time_to_eat);
	while (!is_dead(phil))
	{
		if (phil->params->meal_max > 0 && phil->meal_count > phil->params->meal_max)
		{
			pthread_mutex_lock(&(phil->params->m_num_shaved));
			phil->params->num_shaved++;
			pthread_mutex_unlock(&(phil->params->m_num_shaved));
			break ;
		}
		take_fork('l', phil);
		if (phil->l_taken)
			take_fork('r', phil);
		if (phil->r_taken && phil->l_taken)
		{
			write_state("is eating", phil);
			ft_usleep(phil->params->time_to_eat);
			phil->meal_count++;
			pthread_mutex_lock(&(phil->m_last_meal));
			phil->last_meal = get_timestamp() - phil->params->start_time;
			pthread_mutex_unlock(&(phil->m_last_meal));
			release_fork('r', phil);
			release_fork('l', phil);
			write_state("is sleeping", phil);
			ft_usleep(phil->params->time_to_sleep);
		}
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_params	params;
	t_phil		*philos;
	t_fork		*forks;
	int			cur;

	if (!init_params(&params, argc, argv))
		exit(EXIT_FAILURE);
	if (!create_philos(&philos, &forks, &params))
		exit(EXIT_FAILURE);
	params.start_time = get_timestamp();
	cur = 0;
	while (cur < params.num)
	{
		pthread_create(&(philos[cur].thread), NULL, philo_life, &(philos[cur]));
		//pthread_detach(philos[cur].thread);
		cur++;
	}
	pthread_create(&(params.death_thread), NULL, check_philos_death, &philos);
	//pthread_detach(params.death_thread);
	cur = 0;
	while (cur < params.num)
	{
		pthread_join(philos[cur].thread, NULL);
		cur++;
	}
	pthread_join(params.death_thread, NULL);

/*
	pthread_mutex_destroy(&(params.console_mutex));
	pthread_mutex_destroy(&(params.m_is_dead));
	pthread_mutex_destroy(&(params.m_num_shaved));
	cur = 0;
	while (cur < params.num)
	{
		pthread_mutex_destroy(&(philos[cur].m_last_meal));
		pthread_mutex_destroy(&(forks[cur].lock));
		cur++;
	}
	*/
	free(philos);
	free(forks);

}

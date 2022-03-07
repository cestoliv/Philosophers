/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 10:05:18 by ocartier          #+#    #+#             */
/*   Updated: 2022/03/07 15:29:24 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	create_threads(t_phil **philos, t_params *params)
{
	int	cur;

	params->start_time = get_timestamp();
	cur = 0;
	while (cur < params->num)
	{
		(*philos)[cur].pid = fork();
		if ((*philos)[cur].pid == 0)
			return (philo_life(&((*philos)[cur])));
		cur++;
	}
	if (pthread_create(&(params->death_thread), NULL,
			check_philos_death, philos))
		return (0);
	return (1);
}

int	wait_threads(t_phil **philos, t_params *params)
{
	int	cur;
	int	return_code;
	int	fstatus;

	cur = 0;
	return_code = 1;
	while (cur < params->num)
	{
		if (waitpid((*philos)[cur].pid, &fstatus, 0) == -1)
			return_code = 0;
		cur++;
	}
	if (pthread_join(params->death_thread, NULL))
		return_code = 0;
	return (return_code);
}

int	is_shaved(t_phil *phil)
{
	int	meal_max;

	meal_max = phil->params->meal_max;
	if ((meal_max > 0 && phil->meal_count > meal_max) || meal_max == 0)
	{
		sem_wait(phil->params->sem_num_shaved);
		phil->params->num_shaved++;
		sem_post(phil->params->sem_num_shaved);
		return (1);
	}
	return (0);
}

int	philo_life(t_phil *phil)
{
	if (phil->pos % 2 != 0)
		ft_usleep(phil->params->time_to_eat);
	while (1)
	{
		if (is_shaved(phil))
			break ;
		take_fork(phil);
		if (phil->params->num <= 1)
			break ;
		take_fork(phil);
		write_state("is eating", phil);
		ft_usleep(phil->params->time_to_eat);
		phil->meal_count++;
		sem_wait(phil->sem_last_meal);
		phil->last_meal = get_timestamp() - phil->params->start_time;
		sem_post(phil->sem_last_meal);
		release_forks_and_sleep(phil);
	}
	return (1);
}

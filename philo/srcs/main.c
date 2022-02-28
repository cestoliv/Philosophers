/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 12:24:42 by ocartier          #+#    #+#             */
/*   Updated: 2022/02/28 10:00:44 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	write_state(char *str, t_phil *phil)
{
	int	cur_time;

	cur_time = get_timestamp() - phil->params->start_time;
	pthread_mutex_lock(&(phil->params->console_mutex));
	ft_printf("%d %d %s\n", cur_time, phil->pos, str);
	pthread_mutex_unlock(&(phil->params->console_mutex));
}

void	take_fork(char fork_name, t_phil *phil)
{
	int		*taken;
	t_fork	*fork;

	taken = &(phil->r_taken);
	fork = phil->r_fork;
	if (fork_name == 'l')
	{
		taken = &(phil->l_taken);
		fork = phil->l_fork;
	}
	pthread_mutex_lock(&(fork->lock));
	if (!(*taken) && !fork->used)
	{
		*taken = 1;
		fork->used = 1;
		pthread_mutex_unlock(&(fork->lock));
		write_state("has taken a fork", phil);
	}
	else
		pthread_mutex_unlock(&(fork->lock));
}

void	release_fork(char fork_name, t_phil *phil)
{
	int		*taken;
	t_fork	*fork;

	taken = &(phil->r_taken);
	fork = phil->r_fork;
	if (fork_name == 'l')
	{
		taken = &(phil->l_taken);
		fork = phil->l_fork;
	}
	pthread_mutex_lock(&(fork->lock));
	*taken = 0;
	fork->used = 0;
	pthread_mutex_unlock(&(fork->lock));
}

void	ft_usleep(long int time_in_ms)
{
	long int	start_time;

	start_time = 0;
	start_time = get_timestamp();
	while ((get_timestamp() - start_time) < time_in_ms)
		usleep(1000);
}

void	*check_philo_death(void *arg)
{
	t_phil	*phil;
	int		cur_time;

	phil = (t_phil *)arg;
	while (1)
	{
		cur_time = get_timestamp() - phil->params->start_time;
		pthread_mutex_lock(&(phil->last_meal_mutex));
		if (cur_time - phil->last_meal > phil->params->time_to_die)
		{
			pthread_mutex_unlock(&(phil->last_meal_mutex));
			pthread_mutex_lock(&(phil->params->console_mutex));
			ft_printf("%d %d died\n", cur_time, phil->pos);
			//write_state("died", phil);
			exit(EXIT_FAILURE);
			pthread_mutex_unlock(&(phil->params->console_mutex));
			return (arg);
		}
		else
			pthread_mutex_unlock(&(phil->last_meal_mutex));
		ft_usleep(5);
	}
}

void *check_philos_death(void *arg)
{
	t_params	*params;
	t_phil		**philos;
	int			cur_time;

	philos = (t_phil **)arg;
	params = philos[0]->params;

	while (1)
	{
		cur_time = get_timestamp() - params->start_time;
		for (int i = 0; i < params->num; i++)
		{
			pthread_mutex_lock(&((*philos)[i].last_meal_mutex));
			if (cur_time - (*philos)[i].last_meal > params->time_to_die)
			{
				pthread_mutex_unlock(&((*philos)[i].last_meal_mutex));
				pthread_mutex_lock(&(params->console_mutex));
				ft_printf("%d %d died\n", cur_time, (*philos)[i].pos);
				//write_state("died", phil);
				exit(EXIT_FAILURE);
				pthread_mutex_unlock(&(params->console_mutex));
			}
			else
				pthread_mutex_unlock(&((*philos)[i].last_meal_mutex));
		}
		ft_usleep(1);
	}
	return (arg);
}

void	*philo_life(void *arg)
{
	t_phil		*phil;
	int			cur_time;
	pthread_t	death_thread;

	phil = (t_phil *)arg;

	//phil->state = PHILO_THINK;
	phil->r_taken = 0;
	phil->l_taken = 0;
	//pthread_mutex_lock(&(phil->last_meal_mutex));

	//pthread_mutex_unlock(&(phil->last_meal_mutex));
	//pthread_create(&(phil->death_thread), NULL, check_philo_death, phil);

	if (phil->pos % 2 != 0)
		ft_usleep(phil->params->time_to_eat);

	while (1)
	{
		// Take forks
		/*
		pthread_mutex_lock(&(phil->l_fork->lock));
		write_state("has taken a fork", phil);
		pthread_mutex_lock(&(phil->r_fork->lock));
		write_state("has taken a fork", phil);
		*/

		/*
		if (phil->pos % 2)
		{
			take_fork('r', phil);
			if (phil->r_taken)
				take_fork('l', phil);
		}
		else
		{
			ft_printf("l%d  ", phil->last_meal);
			take_fork('l', phil);
			if (phil->l_taken)
				take_fork('r', phil);
		}
		*/
		take_fork('l', phil);
		if (phil->l_taken)
			take_fork('r', phil);
		if (phil->r_taken && phil->l_taken)
		{
			//ft_printf("t");
			// Eat
			write_state("is eating", phil);
			ft_usleep(phil->params->time_to_eat);
			pthread_mutex_lock(&(phil->last_meal_mutex));
			phil->last_meal = get_timestamp() - phil->params->start_time;
			pthread_mutex_unlock(&(phil->last_meal_mutex));
			// Release forks
			//pthread_mutex_unlock(&(phil->l_fork->lock));
			//pthread_mutex_unlock(&(phil->r_fork->lock));
				//release_fork('l', phil);
				//release_fork('r', phil);
			release_fork('r', phil);
			release_fork('l', phil);

			// Sleep
			write_state("is sleeping", phil);
			ft_usleep(phil->params->time_to_sleep);
		}
/*
		if ((get_timestamp() - phil->params->start_time) - phil->last_meal > phil->params->time_to_die)
		{
			write_state("died", phil);
			exit(EXIT_FAILURE);
		}

		*/
		ft_usleep(5);
		/*
		if (phil->state == PHILO_THINK)
		{

			if (phil->pos % 2)
			{
				take_fork('r', phil, cur_time);
				if (phil->r_taken)
					take_fork('l', phil, cur_time);
			}
			else
			{

				take_fork('l', phil, cur_time);
				if (phil->l_taken)
					take_fork('r', phil, cur_time);
			}
			if (phil->r_taken && phil->l_taken)
			{
				phil->state = PHILO_EAT;
				phil->last_action = cur_time;
				pthread_mutex_lock(&(phil->params->console_mutex));
				ft_printf("%d %d is eating\n", cur_time, phil->pos);
				pthread_mutex_unlock(&(phil->params->console_mutex));
			}
		}

		if (phil->state == PHILO_EAT)
		{
			ft_usleep(phil->params->time_to_eat);
			cur_time = get_timestamp() - phil->params->start_time;
			phil->state = PHILO_SLEEP;
			phil->last_action = cur_time;
			pthread_mutex_lock(&(phil->last_meal_mutex));
			phil->last_meal = cur_time;
			pthread_mutex_unlock(&(phil->last_meal_mutex));
			release_fork('r', phil);
			release_fork('l', phil);
			pthread_mutex_lock(&(phil->params->console_mutex));
			ft_printf("%d %d is sleeping\n", cur_time, phil->pos);
			pthread_mutex_unlock(&(phil->params->console_mutex));
		}

		if (phil->state == PHILO_SLEEP)
		{
			ft_usleep(phil->params->time_to_sleep);
			cur_time = get_timestamp() - phil->params->start_time;
			phil->state = PHILO_THINK;
			phil->last_action = cur_time;
			pthread_mutex_lock(&(phil->params->console_mutex));
			ft_printf("%d %d is thinking\n", cur_time, phil->pos);
			pthread_mutex_unlock(&(phil->params->console_mutex));
		}
		*/
	}
	//pthread_detach(phil->death_thread);
	return (arg);
}

int	create_philos(t_phil **philos, t_fork **forks, t_params	*params)
{
	int	cur;

	*philos = malloc(sizeof(t_phil) * params->num);
	if (!(*philos))
		return (0);
	*forks = malloc(sizeof(t_fork) * params->num);
	if (!(*forks))
	{
		free(*philos);
		return (0);
	}
	cur = 0;
	while (cur < params->num)
	{
		(*philos)[cur].params = params;
		(*philos)[cur].pos = cur;
		(*philos)[cur].last_meal = 0;
		(*philos)[cur].r_fork = &((*forks)[cur]);
		if (cur == params->num - 1)
			(*philos)[cur].l_fork = &((*forks)[0]);
		else
			(*philos)[cur].l_fork = &((*forks)[cur + 1]);
		(*forks)[cur].state = FORK_DIRTY;
		(*forks)[cur].used = 0;
		pthread_mutex_init(&((*philos)[cur].last_meal_mutex), NULL);
		pthread_mutex_init(&((*forks)[cur].lock), NULL);
		cur++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_params	params;
	t_phil		*philos;
	t_fork		*forks;
	int			cur;
	int			philos_num;

	params.num = ft_atoi(argv[1]);
	params.time_to_die = ft_atoi(argv[2]);
	params.time_to_eat = ft_atoi(argv[3]);
	params.time_to_sleep = ft_atoi(argv[4]);
	if (!create_philos(&philos, &forks, &params))
		exit(EXIT_FAILURE);

	pthread_mutex_init(&(params.console_mutex), NULL);
	params.start_time = get_timestamp();

	for (int i = 0; i < params.num; i++)
	{
		pthread_create(&(philos[i].thread), NULL, philo_life, &(philos[i]));
	}
	//ft_usleep(100);
	pthread_t death_thread;
	pthread_create(&death_thread, NULL, check_philos_death, &philos);
	/*
	for (int i = 0; i < params.num; i++)
	{
		pthread_create(&(philos[i].death_thread), NULL, check_philo_death, &(philos[i]));
	}
	*/
	for (int i = 0; i < params.num; i++)
	{
		pthread_join(philos[i].thread, NULL);
	}
}

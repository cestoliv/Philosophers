/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 12:24:42 by ocartier          #+#    #+#             */
/*   Updated: 2022/02/15 12:10:59 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"
/*
#define NB_THREAD 2
#define INCREMENT_LIMIT 10

pthread_t threads[NB_THREAD];

typedef struct mutex_data {
	int data;
	pthread_mutex_t mutex;
} mutex_data;

void * job(void *arg) {
	mutex_data *md = (mutex_data*) arg;
	pthread_t tid = pthread_self();
	while ((*md).data < INCREMENT_LIMIT) {
		pthread_mutex_lock(&(*md).mutex);
		(*md).data++;
		pthread_mutex_unlock(&(*md).mutex);
		printf("thread [ %ld ] data [ %i ]\n", tid, (*md).data);
		sleep(1);
	}
	printf("Fin du thread %ld\n", tid);
	pthread_exit(NULL);
}*/

long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	take_fork(char fork_name, t_phil *phil, int cur_time)
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
	if (!(*taken) && !fork->used)
	{
		pthread_mutex_lock(&(fork->lock));
		*taken = 1;
		fork->used = 1;
		pthread_mutex_unlock(&(fork->lock));
		pthread_mutex_lock(&(phil->params->console_mutex));
		ft_printf("%d %d has taken a fork\n", cur_time, phil->pos);
		pthread_mutex_unlock(&(phil->params->console_mutex));
	}
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

void	*philo_life(void *arg)
{
	t_phil	*phil;
	int		cur_time;

	phil = (t_phil *)arg;
	phil->last_meal = get_timestamp() - phil->params->start_time;
	phil->state = PHILO_THINK;
	phil->r_taken = 0;
	phil->l_taken = 0;
	while (1)
	{
		cur_time = get_timestamp() - phil->params->start_time;
		if (cur_time - phil->last_meal > phil->params->time_to_die)
		{
			pthread_mutex_lock(&(phil->params->console_mutex));
			ft_printf("%d %d died\n", cur_time, phil->pos);
			pthread_mutex_unlock(&(phil->params->console_mutex));
			exit(EXIT_FAILURE);
			return (arg);
		}

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
			if (cur_time - phil->last_action > phil->params->time_to_eat)
			{
				phil->state = PHILO_SLEEP;
				phil->last_action = cur_time;
				phil->last_meal = cur_time;

				release_fork('r', phil);
				release_fork('l', phil);

				pthread_mutex_lock(&(phil->params->console_mutex));
				ft_printf("%d %d is sleeping\n", cur_time, phil->pos);
				pthread_mutex_unlock(&(phil->params->console_mutex));
			}
		}

		if (phil->state == PHILO_SLEEP)
		{
			if (cur_time - phil->last_action > phil->params->time_to_sleep)
			{
				phil->state = PHILO_THINK;
				phil->last_action = cur_time;
				pthread_mutex_lock(&(phil->params->console_mutex));
				ft_printf("%d %d is thinking\n", cur_time, phil->pos);
				pthread_mutex_unlock(&(phil->params->console_mutex));
			}
		}
		usleep(10);
	}
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
		(*philos)[cur].r_fork = &((*forks)[cur]);
		if (cur == params->num - 1)
			(*philos)[cur].l_fork = &((*forks)[0]);
		else
			(*philos)[cur].l_fork = &((*forks)[cur + 1]);
		(*forks)[cur].state = FORK_DIRTY;
		(*forks)[cur].used = 0;
		pthread_mutex_init(&((*forks)[cur].lock), NULL);
		cur++;
	}
	return (1);
}

int	main()
{
	t_params	params;
	t_phil		*philos;
	t_fork		*forks;
	int			cur;
	int			philos_num;

	params.num = 20;
	params.time_to_die = 400;
	params.time_to_eat = 100;
	params.time_to_sleep = 100;
	if (!create_philos(&philos, &forks, &params))
		exit(EXIT_FAILURE);
	/*
	cur = 0;
	while (cur < params.num)
	{
		ft_printf("Philosopher %d :\n\tlast meal : %d\n\tright fork : %x\n\tleft fork : %x\n",
			cur + 1, philos[cur].last_meal, philos[cur].r_fork, philos[cur].l_fork);
		cur++;
	}
	*/

	params.start_time = get_timestamp();
	pthread_mutex_init(&(params.console_mutex), NULL);
	for (int i = 0; i < params.num; i++) {
		int err = pthread_create(&(philos[i].thread), NULL, philo_life, &(philos[i]));
		//sleep(1);
		if (err != 0) {
			//ft_printf("Echec de la création du thread: [%s]", strerror(err));
			break;
		}
		//ft_printf("Création du thread numéro %x\n", &(philos[i].thread));
	}
	for (int i = 0; i < params.num; i++) {
		pthread_join(philos[i].thread, NULL);
	}

	/*
	mutex_data md;
	md.data = 0;
	if (pthread_mutex_init(&md.mutex, NULL) != 0) {
		printf("\n mutex init failed\n");
		return EXIT_FAILURE;
	}
	for (int i = 0; i < NB_THREAD; i++) {
		int err = pthread_create(&threads[i], NULL, job, &md);
		if (err != 0) {
			printf("Echec de la création du thread: [%s]", strerror(err));
			break;
		}
		printf("Création du thread numéro %ld\n", threads[i]);
	}
	for (int i = 0; i < NB_THREAD; i++) {
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&md.mutex);
	return EXIT_SUCCESS;
	*/
}

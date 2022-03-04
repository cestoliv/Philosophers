/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 12:24:34 by ocartier          #+#    #+#             */
/*   Updated: 2022/03/04 14:41:41 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <semaphore.h>
# include <time.h>
# include <sys/time.h>
# include <stdio.h>
# include "../libft/include/libft.h"

typedef struct s_params
{
	int				num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meal_max;
	long			start_time;
	pthread_mutex_t	console_mutex;
	int				num_shaved;
	pthread_mutex_t	m_num_shaved;
	int				is_dead;
	pthread_mutex_t	m_is_dead;
	pthread_t		death_thread;
	sem_t			*sem_forks;
}	t_params;

typedef struct s_phil
{
	pid_t			pid;
	int				pos;
	long			last_meal;
	pthread_mutex_t	m_last_meal;
	int				meal_count;
	int				r_taken;
	int				l_taken;
	t_params		*params;
}	t_phil;

// death.c
int		is_dead(t_phil *phil);
void	*check_philos_death(void *arg);
int		stop_threads(t_phil *phil);
// forks.c
void	take_fork(char fork_name, t_phil *phil);
void	release_fork(char fork_name, t_phil *phil);
void	take_forks(t_phil *phil);
void	release_forks_and_sleep(t_phil *phil);
// init.c
int		create_philos(t_phil **philos, t_params	*params);
int		init_params(t_params *params, int argc, char **argv);
// main.c
void	*philo_life(void *arg);
// threads.c
int		create_threads(t_phil **philos, t_params *params);
int		wait_threads(t_phil **philos, t_params *params);
// utils.c
void	ft_usleep(long int time_in_ms);
void	write_state(char *str, t_phil *phil);
long	get_timestamp(void);
#endif

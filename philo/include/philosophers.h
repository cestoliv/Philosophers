/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 12:24:34 by ocartier          #+#    #+#             */
/*   Updated: 2022/02/28 08:47:51 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <time.h>
# include <sys/time.h>
# include "../libft/include/libft.h"

# define FORK_CLEAR 0
# define FORK_DIRTY 1
# define PHILO_EAT 0
# define PHILO_SLEEP 1
# define PHILO_THINK 2
# define PHILO_WAIT_FORK 3

typedef struct s_params
{
	int				num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			start_time;
	pthread_mutex_t	console_mutex;
}	t_params;

typedef struct s_fork
{
	int	state;
	int	used;
	pthread_mutex_t	lock;
}	t_fork;

typedef struct s_phil
{
	pthread_t	thread;
	pthread_t	death_thread;
	int			pos;
	int			state;
	long		last_meal;
	pthread_mutex_t	last_meal_mutex;
	long		last_action;
	t_fork		*r_fork;
	int			r_taken;
	t_fork		*l_fork;
	int			l_taken;
	t_params	*params;
}	t_phil;
#endif

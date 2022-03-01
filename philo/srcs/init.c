/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 10:38:23 by ocartier          #+#    #+#             */
/*   Updated: 2022/03/01 10:53:35 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

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
		(*philos)[cur].r_taken = 0;
		(*philos)[cur].l_taken = 0;
		if (cur == params->num - 1)
			(*philos)[cur].l_fork = &((*forks)[0]);
		else
			(*philos)[cur].l_fork = &((*forks)[cur + 1]);
		(*forks)[cur].used = 0;
		pthread_mutex_init(&((*philos)[cur].m_last_meal), NULL);
		pthread_mutex_init(&((*forks)[cur].lock), NULL);
		cur++;
	}
	return (1);
}

int	init_params(t_params *params, char **argv)
{
	params->num = ft_atoi(argv[1]);
	params->time_to_die = ft_atoi(argv[2]);
	params->time_to_eat = ft_atoi(argv[3]);
	params->time_to_sleep = ft_atoi(argv[4]);
	params->is_dead = 0;
	pthread_mutex_init(&(params->console_mutex), NULL);
	pthread_mutex_init(&(params->m_is_dead), NULL);
	return (1);
}

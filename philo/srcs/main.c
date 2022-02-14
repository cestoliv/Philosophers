/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 12:24:42 by ocartier          #+#    #+#             */
/*   Updated: 2022/02/14 12:55:34 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	*thread_1(void *arg)
{
	(void)arg;
	ft_printf("Nous sommes dans le thread.\n");
	pthread_exit(EXIT_SUCCESS);
}

int	main(void)
{
	pthread_t	thread1;

	printf("Avant la création du thread.\n");
	pthread_create(&thread1, NULL, thread_1, NULL);
	pthread_join(thread1, NULL);
	printf("Après la création du thread.\n");
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 10:40:26 by ocartier          #+#    #+#             */
/*   Updated: 2022/03/02 10:51:38 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>

void	*thread(void *arg)
{
	return (NULL);
}

int	main()
{
	pthread_t	thid;

	pthread_create(&thid, NULL, thread, NULL);
	//pthread_detach(thid);
	pthread_join(thid, NULL);
}

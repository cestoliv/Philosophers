/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocartier <ocartier@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 12:24:42 by ocartier          #+#    #+#             */
/*   Updated: 2022/02/14 15:06:47 by ocartier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

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
}

int main() {
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
}

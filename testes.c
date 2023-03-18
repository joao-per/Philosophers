/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 15:07:13 by joao-per          #+#    #+#             */
/*   Updated: 2023/03/18 15:07:13 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "philo.h"

unsigned long	get_timestamp(unsigned long t_start)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000) - t_start;
}

void	print_state(t_philo *philo, const char *state)
{
	pthread_mutex_lock(philo->print);
	printf("%lu %d %s\n", get_timestamp(philo->t_start), philo->id, state);
	pthread_mutex_unlock(philo->print);
}


void	*philo_thread(void *arg)
{
	t_philo	*philo = (t_philo *)arg;
	unsigned long last_meal_timestamp = get_timestamp();

	while (1)
	{
		if (philo->eat_count == philo->max_eat)
			break ;
		if (philo->id % 2 == 1)
		{
			pthread_mutex_lock(philo->left_fork);
			print_state(philo->id, "has taken a fork");
			pthread_mutex_lock(philo->right_fork);
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
			print_state(philo->id, "has taken a fork");
			pthread_mutex_lock(philo->left_fork);
		}
		print_state(philo->id, "has taken a fork");

		print_state(philo->id, "is eating");
		usleep(philo->time_to_eat * 1000);
		philo->eat_count++;
		last_meal_timestamp = get_timestamp();

		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);

		print_state(philo->id, "is sleeping");
		usleep(philo->time_to_sleep * 1000);

		print_state(philo->id, "is thinking");

		if (get_timestamp() - last_meal_timestamp >= philo->time_to_die)
		{
			print_state(philo->id, "died");
			break ;
		}
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	pthread_mutex_t	*forks;
	pthread_t		*threads;
	t_philo			*philo;
	int				i;
	int				num_philo;

	num_philo = atoi(av[1]);
	threads = (pthread_t *) malloc(num_philo * sizeof(pthread_t));
	philo = (t_philo *) malloc(num_philo * sizeof(t_philo));
	forks = (pthread_mutex_t *) malloc(num_philo * sizeof(pthread_mutex_t));
	if (ac < 5 || ac > 6)
	{
		printf("Incorrect number of arguments\n");
		return (1);
	}
	i = 0;
	while (i < num_philo)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < num_philo)
	{
		philo[i].id = i + 1;
		philo[i].time_to_die = atoi(av[2]);
		philo[i].time_to_eat = atoi(av[3]);
		philo[i].time_to_sleep = atoi(av[4]);
		philo[i].eat_count = 0;
		if (ac == 6)
			philo[i].max_eat = atoi(av[5]);
		else
			philo[i].max_eat = -1;
		philo[i].left_fork = &forks[i];
		philo[i].right_fork = &forks[(i + 1) % num_philo];
		if (pthread_create(&threads[i], NULL, philo_thread, &philo[i]) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < num_philo)
	{
		if (pthread_join(threads[i], NULL) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < num_philo)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(threads);
	free(philo);
	free(forks);

}

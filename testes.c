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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "philo.h"

unsigned long	get_timestamp(unsigned long t_start)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - t_start);
}

void	print_state(t_philo *philo, const char *state)
{
	pthread_mutex_lock(&philo->info->print);
	if (!philo->info->death_occurred)
	{
		printf("%lu %d %s\n", get_timestamp(philo->info->t_start),
			philo->id, state);
	}
	pthread_mutex_unlock(&philo->info->print);
}

void	*philo_thread(void *arg)
{
	t_philo			*philo;
	unsigned long	last_meal;
	int				death_occurred;
	int             num_philo;

	philo = (t_philo *)arg;
	last_meal = get_timestamp(philo->info->t_start);
	num_philo = philo->info->num_philo;

	while (1)
	{
		pthread_mutex_lock(&philo->info->print);
		death_occurred = philo->info->death_occurred;
		pthread_mutex_unlock(&philo->info->print);

		if (death_occurred)
		{
			break ;
		}

		if (get_timestamp(philo->t_start) - last_meal >= philo->time_to_die)
		{
			pthread_mutex_lock(&philo->info->print);
			if (!philo->info->death_occurred)
			{
				philo->info->death_occurred = 1;
				printf("%lu %d died\n", get_timestamp(philo->info->t_start), philo->id);
			}
			pthread_mutex_unlock(&philo->info->print);
			break ;
		}

		if (philo->eat_count == philo->max_eat)
			break;

		if (num_philo == 1)
		{
			printf("%lu %d has taken a fork\n", get_timestamp(philo->info->t_start), philo->id);
			usleep(philo->time_to_die * 1000);
			continue;
		}

		if (philo->id % 2 == 1)
		{
			pthread_mutex_lock(philo->left_fork);
			print_state(philo, "has taken a fork");
			pthread_mutex_lock(philo->right_fork);
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
			print_state(philo, "has taken a fork");
			pthread_mutex_lock(philo->left_fork);
		}
		print_state(philo, "has taken a fork");

		last_meal = get_timestamp(philo->t_start);
		print_state(philo, "is eating");
		usleep(philo->time_to_eat * 1000);
		philo->eat_count++;

		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);

		print_state(philo, "is sleeping");
		usleep(philo->time_to_sleep * 1000);

		print_state(philo, "is thinking");
	}
	return (NULL);
}



int	main(int ac, char **av)
{
	pthread_mutex_t	*forks;
	pthread_t		*threads;
	t_philo			*philo;
	t_info			info;
	int				i;
	int				num_philo;
	unsigned long	t_start;
	pthread_mutex_t print_mutex;

	pthread_mutex_init(&info.print, NULL);
	info.t_start = get_timestamp(0);
	info.death_occurred = 0;
	num_philo = ft_atol(av[1]);
	info.num_philo = num_philo;
	threads = (pthread_t *) malloc(num_philo * sizeof(pthread_t));
	philo = (t_philo *) malloc(num_philo * sizeof(t_philo));
	forks = (pthread_mutex_t *) malloc(num_philo * sizeof(pthread_mutex_t));
	philo->is_dead = 0;
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
	t_start = get_timestamp(0);
	pthread_mutex_init(&print_mutex, NULL);
	i = 0;
	while (i < num_philo)
	{
		philo[i].id = i + 1;
		philo[i].time_to_die = ft_atol(av[2]);
		philo[i].time_to_eat = ft_atol(av[3]);
		philo[i].time_to_sleep = ft_atol(av[4]);
		philo[i].eat_count = 0;
		if (ac == 6)
			philo[i].max_eat = ft_atol(av[5]);
		else
			philo[i].max_eat = -1;
		philo[i].left_fork = &forks[i];
		philo[i].right_fork = &forks[(i + 1) % num_philo];
		philo[i].t_start = t_start; // Set t_start for each philosopher
		philo[i].print = &print_mutex; // Set print mutex for each philosopher
		philo[i].info = &info;
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
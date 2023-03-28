/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 18:29:25 by joao-per          #+#    #+#             */
/*   Updated: 2023/03/27 18:29:25 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*init_philo(int ac, char **av)
{
	t_philo *philo;
	int i;
	int num_philo;

	num_philo = ft_atol(av[1]);
	philo = (t_philo *)malloc(num_philo * sizeof(t_philo));
	philo->is_dead = 0;
	i = 0;
	while (i < num_philo)
	{
		philo[i].id = i + 1;
		philo[i].time_to_die = ft_atol(av[2]);
		philo[i].last_meal = 0;
		philo[i].time_to_eat = ft_atol(av[3]);
		philo[i].time_to_sleep = ft_atol(av[4]);
		philo[i].eat_count = 0;
		if (ac == 6)
			philo[i].max_eat = ft_atol(av[5]);
		else
			philo[i].max_eat = -1;
		i++;
	}
	init_forks(philo, num_philo, -1);
	return(philo);
}

t_philo	*init_forks(t_philo *philo, int num_philo, int i)
{
	pthread_mutex_t *forks;

	forks = (pthread_mutex_t *)malloc(num_philo * sizeof(pthread_mutex_t));
	while (++i < num_philo)
		pthread_mutex_init(&forks[i], NULL);
	i = -1;
	while (++i < num_philo)
	{
		if (i + 1 != num_philo)
		{
			philo[i].left_fork = &forks[i];
			philo[i].right_fork = &forks[(i + 1) % num_philo];
		}
		else
		{
			philo[i].left_fork = &forks[(i + 1) % num_philo];
			philo[i].right_fork = &forks[i];
		}
	}
	init_threads(philo, num_philo, -1);
	while (++i < num_philo)
		pthread_mutex_destroy(&forks[i]);
	free(forks);
	return(philo);
}

int	init_threads(t_philo *philo, int num_philo, int i)
{
	pthread_t *threads;
	t_info		info;

	pthread_mutex_init(&info.print, NULL);
	info.t_start = get_timestamp(0);
	info.death_occurred = 0;
	info.max_eats = 0;
	info.num_philo = num_philo;
	threads = (pthread_t *)malloc(num_philo * sizeof(pthread_t));
	while (++i < num_philo)
	{
		philo[i].t_start = info.t_start;
		philo[i].info = &info;
		if (pthread_create(&threads[i], NULL, philo_thread, &philo[i]) != 0)
			return (1);
	}
	program_checker(philo, num_philo);
	i = -1;
	while (++i < num_philo)
	{
		if (pthread_join(threads[i], NULL) != 0)
			return (1);
	}
	free(threads);
}
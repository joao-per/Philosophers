/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 23:26:39 by joao-per          #+#    #+#             */
/*   Updated: 2023/05/13 23:26:39 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_nbr_eats(t_philo *philo)
{
	int	nbr_eats;

	pthread_mutex_lock(&philo->info->print);
	nbr_eats = philo->eat_count;
	pthread_mutex_unlock(&philo->info->print);
	if (nbr_eats < philo->max_eat || philo->max_eat == -1)
		return (0);
	philo->info->max_eats = 1;
	return (1);
}

void	pickup_forks(t_philo *philo)
{
	pthread_mutex_t	*fork1;
	pthread_mutex_t	*fork2;

	fork1 = philo->left_fork;
	fork2 = philo->right_fork;
	pthread_mutex_lock(fork1);
	print_state(philo, "has taken a fork");
	if (philo->info->num_philo == 1)
		return ;
	pthread_mutex_lock(fork2);
	print_state(philo, "has taken a fork");
	pthread_mutex_lock(&philo->info->print);
	philo->last_meal = get_timestamp(philo->t_start);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->info->print);
}

void random_usleep(int min_time, int max_time)
{
    int sleep_time = (rand() % (max_time - min_time + 1)) + min_time;
    usleep(sleep_time);
}

void	start_eating(t_philo *philo)
{
	print_state(philo, "is eating");
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	usleep(500);
}

void	*philo_thread(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pickup_forks(philo);
		if (philo->info->num_philo == 1)
			return (0);
		start_eating(philo);
		pthread_mutex_lock(&philo->info->print);
		if (philo->eat_count == philo->max_eat || philo->info->death_occurred)
		{
			pthread_mutex_unlock(&philo->info->print);
			break ;
		}
		pthread_mutex_unlock(&philo->info->print);
		print_state(philo, "is sleeping");
		usleep(philo->time_to_sleep * 1000);
		print_state(philo, "is thinking");
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_philo	*philo;

	if (arg_checker(ac, av) == 0)
		return (1);
	philo = init_philo(ac, av);
	free(philo);
}

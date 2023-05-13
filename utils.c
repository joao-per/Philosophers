/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 23:26:29 by joao-per          #+#    #+#             */
/*   Updated: 2023/05/13 23:26:29 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atol(const char *str)
{
	long int	result;
	int			i;
	int			negative;

	i = 0;
	negative = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			negative = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (str[i] - 48) + (result * 10);
		i++;
	}
	if (negative == -1)
		result = -result;
	if (result > INT_MAX || result < INT_MIN)
		return (6969696969);
	return (result);
}

void	program_checker(t_philo *philo, int num_philo)
{
	int	max_eats;
	int	death;
	int	i;

	max_eats = 0;
	death = 0;
	while (1 && !max_eats && !death)
	{
		i = 0;
		while (i < num_philo)
		{
			if (should_philosopher_die(&philo[i]))
				death = 1;
			if (check_nbr_eats(&philo[i]))
				max_eats = 1;
			i++;
		}
	}
}

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
		printf("%lu %d %s\n", get_timestamp(philo->info->t_start), philo->id,
			state);
	}
	pthread_mutex_unlock(&philo->info->print);
}

int	should_philosopher_die(t_philo *philo)
{
	int	last_eated;

	pthread_mutex_lock(&philo->info->print);
	last_eated = get_timestamp(philo->t_start) - philo->last_meal;
	pthread_mutex_unlock(&philo->info->print);
	if (last_eated >= philo->time_to_die)
	{
		pthread_mutex_lock(&philo->info->print);
		if (!philo->info->death_occurred)
		{
			philo->info->death_occurred = 1;
			printf("%lu %d died\n", get_timestamp(philo->info->t_start),
				philo->id);
		}
		pthread_mutex_unlock(&philo->info->print);
		return (1);
	}
	return (0);
}

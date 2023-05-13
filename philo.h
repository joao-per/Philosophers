/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao-per <joao-per@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 12:12:48 by joao-per          #+#    #+#             */
/*   Updated: 2023/03/29 12:12:48 by joao-per         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>

typedef struct s_info
{
	unsigned long	t_start;
	pthread_mutex_t	print;
	int				death_occurred;
	int				max_eats;
	int				num_philo;
}				t_info;

typedef struct s_philo
{
	int				id;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_count;
	int				last_meal;
	int				max_eat;
	int				is_dead;
	unsigned long	t_start;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*print;
	t_info			*info;
}			t_philo;

t_philo			*init_forks(t_philo *philo, int num_philo, int i);
t_philo			*init_philo(int ac, char **av);
long			ft_atol(const char *str);
int				should_philosopher_die(t_philo *philo);
void			program_checker(t_philo *philo, int num_philo);
int				check_nbr_eats(t_philo *philo);
int				init_threads(t_philo *philo, int num_philo, int i);
unsigned long	get_timestamp(unsigned long t_start);
void			print_state(t_philo *philo, const char *state);
void			*philo_thread(void *arg);
int				should_philosopher_die(t_philo *philo);
int				arg_checker(int ac, char **av);

#endif
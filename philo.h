#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_info
{
	unsigned long	t_start;
	pthread_mutex_t	print;
	int				death_occurred;
	int             num_philo;
}				t_info;

typedef struct s_philo
{
	int				id;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_count;
	int				max_eat;
	int				is_dead;
	unsigned long	t_start;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*print;
	t_info			*info;
}			t_philo;

long	ft_atol(const char *str);

#endif
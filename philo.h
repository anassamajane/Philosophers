#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_rules
{
	int	num_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	must_eat_count;
	int	full_philos;
	int	someone_died;
	long long	start_time;
	pthread_mutex_t	writing;
	pthread_mutex_t	meal_check;
	pthread_mutex_t	death_check;
	pthread_mutex_t	meal_count_lock;
	pthread_mutex_t	*forks;
}	t_rules;

typedef struct s_philo
{
	int	id;
	int	eat_count;
	int	done_eating;
	long long	last_meal;
	pthread_t	thread_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_rules	*rules;
}	t_philo;

void	*routine(void *args);
int	parse_args(int ac, char **av, t_rules *rules);
t_philo	*init_philos(t_rules *rules);
long long       get_time(void);
int     create_threads(t_rules *rules, t_philo *philos);
void    print_action(t_philo *philo, char *action);

#endif

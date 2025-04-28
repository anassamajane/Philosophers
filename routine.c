#include "philo.h"

long long	get_time(void)
{
	struct timeval	time;
	long long	mls;

	gettimeofday(&time, NULL);
	mls = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (mls);
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(philo->left_fork);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->right_fork);
	}
}

void	put_down_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	print_action(t_philo *philo, char *action)
{
	long long	timestamp;

	timestamp = get_time() - philo->rules->start_time;
	pthread_mutex_lock(&philo->rules->writing);
	printf("%lld %d %s\n", timestamp, philo->id, action);
	pthread_mutex_unlock(&philo->rules->writing);
}

void	*routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (1)
	{
		take_forks(philo);
		print_action(philo, "has taken a fork");
		print_action(philo, "has taken a fork");
		print_action(philo ,"is eating");
		usleep(philo->rules->time_to_eat * 1000);
		put_down_forks(philo);
		print_action(philo ,"is sleeping");
		usleep(philo->rules->time_to_sleep * 1000);
		print_action(philo, "is thinking");
	}
	return (NULL);
}

int	create_threads(t_rules *rules, t_philo *philos)
{
	int	i;

	i = 0;
	rules->start_time = get_time();
	while (i < rules->num_philos)
	{
		philos[i].last_meal = rules->start_time;
		if (pthread_create(&philos[i].thread_id, NULL, routine, &philos[i]) != 0)
		{
			printf("Error creating thread %d\n", i);
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_rules	rules;
	t_philo	*philos;
	int	i;

	if (parse_args(ac, av, &rules))
		return (1);
	philos = init_philos(&rules);
	if (!philos)
		return (1);
	if (create_threads(&rules, philos))
		return (1);
	i = 0;
	while (i < rules.num_philos)
		pthread_join(philos[i++].thread_id, NULL);
}

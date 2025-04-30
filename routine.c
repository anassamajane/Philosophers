#include "philo.h"

/*void	smart_sleep(long long time, t_philo *philo)
{
	long long	start;

	start = get_time();
	while (1)
	{
		pthread_mutex_lock(&philo->rules->death_check);
		if (philo->rules->someone_died)
		{
			pthread_mutex_unlock(&philo->rules->death_check);
			break;
		}
		pthread_mutex_unlock(&philo->rules->death_check);
		if (get_time() - start >= time)
			break;
		usleep(500);
	}
}*/

/*long long	get_time(void)
{
	struct timeval	time;
	long long	mls;

	gettimeofday(&time, NULL);
	mls = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (mls);
}*/

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
	}
}

void	put_down_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

/*void	print_action(t_philo *philo, char *action)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->rules->death_check);
	if (philo->rules->someone_died)
	{
		pthread_mutex_unlock(&philo->rules->death_check);
		return ;
	}
	pthread_mutex_unlock(&philo->rules->death_check);
	timestamp = get_time() - philo->rules->start_time;
	pthread_mutex_lock(&philo->rules->writing);
	printf("%lld %d %s\n", timestamp, philo->id, action);
	pthread_mutex_unlock(&philo->rules->writing);
}*/

/*void	*monitor(t_rules *rules, t_philo *philos)
{
	rules = philos[0].rules;
	int		i;

	while (1)
	{
		i = 0;
		while (i < rules->num_philos)
		{
			pthread_mutex_lock(&rules->meal_check);
			long long time_since_meal = get_time() - philos[i].last_meal;
			pthread_mutex_unlock(&rules->meal_check);

			if (time_since_meal > rules->time_to_die)
			{
				pthread_mutex_lock(&rules->death_check);
				if (!rules->someone_died && rules->full_philos != rules->num_philos)///////////
				{
					rules->someone_died = 1;
					pthread_mutex_unlock(&rules->death_check);

					pthread_mutex_lock(&rules->writing);
					printf("%lld %d died\n", get_time() - rules->start_time, philos[i].id);
					pthread_mutex_unlock(&rules->writing);
				}
				else
					pthread_mutex_unlock(&rules->death_check);
				return (NULL);
			}
			i++;
		}
		if (rules->must_eat_count > 0)
		{
			pthread_mutex_lock(&rules->meal_count_lock);
			if (rules->full_philos == rules->num_philos)
			{
				pthread_mutex_unlock(&rules->meal_count_lock);
				pthread_mutex_lock(&rules->death_check);
				rules->someone_died = 1;
				pthread_mutex_unlock(&rules->death_check);
				return (NULL);
			}
			pthread_mutex_unlock(&rules->meal_count_lock);
		}
		usleep(1000); // small delay to reduce CPU usage
	}
}*/

void	*one_philo_case(t_philo *philo)
{
	if (philo->rules->num_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
		while (1)
		{
			pthread_mutex_lock(&philo->rules->death_check);
			if (philo->rules->someone_died)
			{
				pthread_mutex_unlock(&philo->rules->death_check);
				break ;
			}
			pthread_mutex_unlock(&philo->rules->death_check);
			usleep(500);
		}
	}
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

int	check_should_exit(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->death_check);
	if (philo->rules->someone_died)
	{
		pthread_mutex_unlock(&philo->rules->death_check);
		return (1);
	}
	pthread_mutex_unlock(&philo->rules->death_check);
	if (philo->done_eating)
		return (1);
	return (0);
}

void	eat_sleep_think(t_philo *philo)
{
	take_forks(philo);
	pthread_mutex_lock(&philo->rules->meal_check);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->rules->meal_check);

	print_action(philo, "is eating");
	smart_sleep(philo->rules->time_to_eat, philo);
	philo->eat_count++;

	if (!philo->done_eating && philo->rules->must_eat_count > 0 && philo->eat_count == philo->rules->must_eat_count)
	{
		pthread_mutex_lock(&philo->rules->meal_count_lock);
		philo->rules->full_philos++;
		philo->done_eating = 1;
		pthread_mutex_unlock(&philo->rules->meal_count_lock);
	}
	put_down_forks(philo);

	print_action(philo, "is sleeping");
	smart_sleep(philo->rules->time_to_sleep, philo);
	print_action(philo, "is thinking");
}


void	*routine(void *args)
{
	t_philo	*philo;

	philo = (void *)args;
	if (philo->rules->num_philos == 1)
	{
		one_philo_case(philo);
		return (NULL);
	}
	while (1)
	{
		if (check_should_exit(philo))
			break ;
		eat_sleep_think(philo);
	}
	return (NULL);
}

/*void	*routine(void *args)
{
	t_philo	*philo;

	philo = (void *)args;
	if (philo->rules->num_philos == 1)
	{
		one_philo_case(philo);
		return (NULL);
	}
	while (1)
	{
		pthread_mutex_lock(&philo->rules->death_check);
		if (philo->rules->someone_died)
		{
			pthread_mutex_unlock(&philo->rules->death_check);
			break ;
		}
		pthread_mutex_unlock(&philo->rules->death_check);
		if (philo->done_eating)
			break ;
		take_forks(philo);

		pthread_mutex_lock(&philo->rules->meal_check);
		philo->last_meal = get_time();
		pthread_mutex_unlock(&philo->rules->meal_check);

		print_action(philo ,"is eating");
		smart_sleep(philo->rules->time_to_eat, philo);
		philo->eat_count++;
		if (!philo->done_eating && philo->rules->must_eat_count != -1 && philo->eat_count == philo->rules->must_eat_count)
		{
			pthread_mutex_lock(&philo->rules->meal_count_lock);
			philo->rules->full_philos++;
			philo->done_eating = 1;
			pthread_mutex_unlock(&philo->rules->meal_count_lock);
		}

		put_down_forks(philo);

		print_action(philo ,"is sleeping");
		smart_sleep(philo->rules->time_to_sleep, philo);
		print_action(philo, "is thinking");
	}
	return (NULL);
}*/

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
	monitor(&rules, philos);
	i = 0;
	while (i < rules.num_philos)
		pthread_join(philos[i++].thread_id, NULL);
	free(philos);
	return (0);
}

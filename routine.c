#include "philo.h"

void eat_sleep_think(t_philo *philo)
{
	// if (philo->id % 2 == 0)
	// {
	// 	pthread_mutex_lock(philo->right_fork);
	// 	print_action(philo, "has taken a fork");
	// 	pthread_mutex_lock(philo->left_fork);
	// 	print_action(philo, "has taken a fork");
	// }
	// else
	// {
	// 	pthread_mutex_lock(philo->left_fork);
	// 	print_action(philo, "has taken a fork");
	// 	pthread_mutex_lock(philo->right_fork);
	// 	print_action(philo, "has taken a fork");
	// }

	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken a fork");
	print_action(philo, "has taken a fork");
	print_action(philo, "is eating");
	philo->last_meal = get_time();
	smart_sleep(philo->rules->time_to_eat, philo);
	philo->eat_count++;

	if (!philo->done_eating && philo->rules->must_eat_count > 0 && philo->eat_count == philo->rules->must_eat_count)
	{
		pthread_mutex_lock(&philo->rules->meal_count_lock);
		philo->rules->full_philos++;
		pthread_mutex_unlock(&philo->rules->meal_count_lock);
		philo->done_eating = 1;
	}
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	print_action(philo, "is sleeping");
	smart_sleep(philo->rules->time_to_sleep, philo);
	print_action(philo, "is thinking");
}

void *routine(void *args)
{
	t_philo *philo;

	philo = (void *)args;
	// if (philo->id % 2 == 0)
	// 	usleep(500);
	if (philo->rules->num_philos == 1)
	{
		print_action(philo, "has taken a fork");
		smart_sleep(philo->rules->time_to_die, philo); // mon doing the job of setting sim_status to 0 !!!!
		return (NULL);
	}
	while (philo->rules->sim_status)
		eat_sleep_think(philo);
	return (NULL);
}

int create_threads(t_rules *rules, t_philo *philos)
{
	int i;

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
		i += 2;
	}
	i = 1;
	while (i < rules->num_philos)
	{
		philos[i].last_meal = rules->start_time;
		if (pthread_create(&philos[i].thread_id, NULL, routine, &philos[i]) != 0)
		{
			printf("Error creating thread %d\n", i);
			return (1);
		}
		i += 2;
	}
	return (0);
}

int main(int ac, char **av)
{
	t_rules rules;
	t_philo philos[MAX_PHILO];
	int i;

	if (parse_args(ac, av, &rules))
		return (1);
	init_philos(&rules, philos);
	if (!philos)
		return (1);
	if (create_threads(&rules, philos))
		return (1);
	monitor(&rules, philos);
	i = 0;
	while (i < rules.num_philos)
		pthread_join(philos[i++].thread_id, NULL);
	if (rules.dead_philo_id != -1)
		printf("%lld %d died\n", get_time() - rules.start_time, rules.dead_philo_id);
	return (0);
}

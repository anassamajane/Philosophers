#include "philo.h"

void eat_sleep_think(t_philo *philo)
{
// take forks
	pthread_mutex_lock(philo->right_fork);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(philo->left_fork);

// update the last eating time
	pthread_mutex_lock(&philo->meal_mutex);//
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->meal_mutex);//

	print_action(philo, "has taken a fork");
	print_action(philo, "is eating");

// Have eaten enough logic
	philo->eat_count++;
	smart_sleep(philo->rules->time_to_eat, philo);
	if (!philo->done_eating && philo->rules->must_eat_count > 0 &&
	philo->eat_count == philo->rules->must_eat_count)
	{
		pthread_mutex_lock(&philo->rules->meal_count_lock);
		philo->rules->full_philos++;
		philo->done_eating = 1;
		pthread_mutex_unlock(&philo->rules->meal_count_lock);
	}
// release forks
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
// sleep and wake-up
	print_action(philo, "is sleeping");
	smart_sleep(philo->rules->time_to_sleep, philo);
	print_action(philo, "is thinking");

// if ttd == (tte + tts) philoes are stealing earch other forks
	if (philo->rules->num_philos % 2 != 0)
		usleep(500);
}

void *routine(void *args)
{
	t_philo *philo;

	philo = (void *)args;
	if (philo->rules->num_philos == 1)
	{
		print_action(philo, "has taken a fork");
		smart_sleep(philo->rules->time_to_die, philo); // mon doing the job of setting sim_status to 0 !!!!
		return (NULL);
	}
	while (get_sim_status(philo->rules))////
		eat_sleep_think(philo);
	return (NULL);
}

int create_threads(t_rules *rules, t_philo *philos)
{
	int i;

	i = 1;
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
	usleep(500);
	i = 0;
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
	if (create_threads(&rules, philos))
		return (1);
	monitor(&rules, philos);
	i = 0;
	while (i < rules.num_philos)
		pthread_join(philos[i++].thread_id, NULL);
	if (rules.dead_philo_id != -1)
		printf("%lld %d died\n", (get_time() - rules.start_time), rules.dead_philo_id);
	cleaning(philos, &rules);
	return (0);
}

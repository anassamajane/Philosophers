#include "philo_bonus.h"

void	eat_sleep_think(t_philo *philo)
{
	while (1)
	{
		sem_wait(philo->rules->forks);
		print_action(philo, "has taken a fork");
		sem_wait(philo->rules->forks);
		print_action(philo, "has taken a fork");
		print_action(philo, "is eating");
		sem_wait(philo->rules->meal_lock);
		philo->last_meal = get_time();
		philo->eat_count++;
		sem_post(philo->rules->meal_lock);
		smart_sleep(philo->rules->time_to_eat, philo);
		if (!philo->done_eating && philo->rules->must_eat_count > 0 && philo->eat_count == philo->rules->must_eat_count)
		{
			sem_wait(philo->rules->meal_lock);
			philo->rules->full_philos++;
			philo->done_eating = 1;
			sem_post(philo->rules->meal_lock);
		}
		sem_post(philo->rules->forks);
		sem_post(philo->rules->forks);
		print_action(philo, "is sleeping");
		smart_sleep(philo->rules->time_to_sleep, philo);
		print_action(philo, "is thinking");
	}
}

int	routine(t_rules *rules, t_philo *philos)
{
	pid_t	pid;
	int	i;

	if (philos->rules->num_philos == 1)
	{
		print_action(philos, "has taken a fork");
		smart_sleep(philos->rules->time_to_die, philos);
		return (1);
	}
	rules->start_time = get_time();
	i = 0;
	while (i < rules->num_philos)
	{
		pid = fork();
		if (pid < 0)
		{
			printf("Fork failed\n");
			return (1);
		}
		else if (pid == 0)
		{
			philos[i].last_meal = rules->start_time;
			eat_sleep_think(&philos[i]);
			exit(0);
		}
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_rules	rules;
	t_philo	philos[MAX_PHILO];
	int	i;

	if (parse_args(ac, av, &rules))
		return (1);
	if (init_semaphores(&rules))
	{
		printf("Failed to initialize semaphores\n");
		return (1);
	}
	init_philos(&rules, philos);
	if (routine(&rules, philos))
		return (1);
	monitor(&rules, philos);
	i = 0;
	while (i < rules.num_philos)
	{
		waitpid(-1, NULL, 0);
		i++;
	}
	if (rules.dead_philo_id != -1)
	{
		sem_wait(rules.sim_lock);
		printf("%lld %d died\n", (get_time() - rules.start_time), rules.dead_philo_id);
		sem_post(rules.sim_lock);
	}
	sem_close(rules.forks);
	sem_close(rules.print);
	sem_close(rules.sim_lock);
	sem_close(rules.meal_lock);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/sim_lock");
	sem_unlink("/meal_lock");
	return (0);
}

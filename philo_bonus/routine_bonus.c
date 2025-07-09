#include "philo_bonus.h"


void	*death_monitor(void *arg)
{
	t_philo	*philo;
	time_t	time_since_meal;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->rules->meal_lock);
		time_since_meal = get_time() - philo->last_meal;
		sem_post(philo->rules->meal_lock);
		if (time_since_meal > philo->rules->time_to_die)
		{
			sem_wait(philo->rules->print);
			printf("%lld %d died\n", get_time() - philo->rules->start_time, philo->id);
			sem_post(philo->rules->death_sem);//Signal death to main process//
			exit(1);
		}
	}
	return (NULL);
}

void	*meal_monitor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->rules->meal_lock);
		if (philo->rules->must_eat_count > 0 && philo->eat_count >= philo->rules->must_eat_count)
		{
			sem_post(philo->rules->meal_lock);
			sem_post(philo->rules->finished_sem);//Signal that this philosopher finished eating//
			exit(0);
		}
		sem_post(philo->rules->meal_lock);
	}
	return (NULL);
}

void	eat_sleep_think(t_philo *philo)
{
	pthread_t	death_thread;
	pthread_t	meal_thread;

	philo->last_meal = get_time();//
	pthread_create(&death_thread, NULL, death_monitor, philo);
	pthread_detach(death_thread);
	if (philo->rules->must_eat_count > 0)
	{
		pthread_create(&meal_thread, NULL, meal_monitor, philo);
		pthread_detach(meal_thread);
	}
	if (philo->id % 2 == 0)
		usleep(500);
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
		smart_sleep(philo->rules->time_to_eat);
		/*if (philo->rules->must_eat_count > 0 && philo->eat_count >= philo->rules->must_eat_count)
		{
			sem_post(philo->rules->forks);
			sem_post(philo->rules->forks);
		}*/
		sem_post(philo->rules->forks);
		sem_post(philo->rules->forks);
		print_action(philo, "is sleeping");
		smart_sleep(philo->rules->time_to_sleep);
		print_action(philo, "is thinking");
	}
}

/*int    handle_single_philo(t_philo *philo)
{
    philo->last_meal = philo->rules->start_time;
    print_action(philo, "has taken a fork");
    smart_sleep(philo->rules->time_to_die);
    print_action(philo, "died");
    return (1);
}*/

void	*monitor_sim(void *arg)
{
	t_rules	*rules;
	int	finished_count;

	rules = (t_rules *)arg;)void *death_moni
	finished_count = 0;
	if (rules->must_eat_count > 0)
	{
		while (finished_count < rules->num_philos)
		{
			sem_wait(rules->finished_sem);
			finished_count++;
		}
		sem_post(rules->death_sem);// All finished eating - signal death to stop simulation//
	}
	return (NULL);
}

int	routine(t_rules *rules, t_philo *philos)
{
	pid_t	pids[MAX_PHILO];
	pthread_t	monitor_thread;
	int	i;

	if (philos->rules->num_philos == 1)
	{
		print_action(philos, "has taken a fork");
		smart_sleep(philos->rules->time_to_die);
		return (1);
	}	
	if (rules->must_eat_count > 0)
	{
		pthread_create(&monitor_thread, NULL, monitor_sim, rules);
		pthread_detach(monitor_thread);
	}
	i = 0;	
	while (i < rules->num_philos)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			printf("Fork failed\n");
			return (1);
		}
		else if (pids[i] == 0)void	*death_monitor(void *arg)
{
	t_philo	*philo;
	time_t	time_since_meal;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->rules->meal_lock);
		time_since_meal = get_time() - philo->last_meal;
		sem_post(philo->rules->meal_lock);
		if (time_since_meal > philo->rules->time_to_die)
		{
			sem_wait(philo->rules->print);
			printf("%lld %d died\n", get_time() - philo->rules->start_time, philo->id);
			sem_post(philo->rules->death_sem);//Signal death to main process//
			exit(1);
		}
	}
	return (NULL);void	*death_monitor(void *arg)
{
	t_philo	*philo;
	time_t	time_since_meal;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->rules->meal_lock);
		time_since_meal = get_time() - philo->last_meal;
		sem_post(philo->rules->meal_lock);
		if (time_since_meal > philo->rules->time_to_die)
		{
			sem_wait(philo->rules->print);
			printf("%lld %d died\n", get_time() - philo->rules->start_time, philo->id);
			sem_post(philo->rules->death_sem);//Signal death to main process//
			exit(1);
		}
	}
	return (NULL);
}
}
		{
			//philos[i].last_meal = rules->start_time;
			philos[i].rules->start_time = get_time();
			eat_sleep_think(&philos[i]);
			exit(0);
		}
		i++;
	}
	sem_wait(rules->death_sem);//Wait for death signal (either actual death or all finished eating)//
	i = 0;
	while (i < rules->num_philos)
	{
		kill(pids[i], SIGKILL);
		i++;
	}
	i = 0;
	while (i < rules->num_philos)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_rules	rules;
	t_philo	philos[MAX_PHILO];

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
	sem_close(rules.forks);
	sem_close(rules.print);
	sem_close(rules.death_sem);
	sem_close(rules.finished_sem);
	sem_close(rules.meal_lock);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/death_sem");
	sem_unlink("/finished_sem");
	sem_unlink("/meal_lock");
	return (0);
}

#include "philo.h"

void	print_death(t_rules *rules, int id)
{
	pthread_mutex_lock(&rules->writing);
	printf("%lld %d died\n", get_time() - rules->start_time, id);
	pthread_mutex_unlock(&rules->writing);
}

void	set_death_flag(t_rules *rules)
{
	pthread_mutex_lock(&rules->death_check);
	rules->someone_died = 1;
	pthread_mutex_unlock(&rules->death_check);
}

int	should_die(t_rules *rules)
{
	int	someone_died;

	pthread_mutex_lock(&rules->death_check);
	someone_died = rules->someone_died;
	pthread_mutex_unlock(&rules->death_check);
	if (someone_died)
		return (0);
	return (1);
}

int	check_philo_death(t_rules *rules, t_philo * philo)
{
	long long	time_since_meal;

	pthread_mutex_lock(&rules->meal_check);
	time_since_meal = get_time() - philo->last_meal;
	pthread_mutex_unlock(&rules->meal_check);
	if (time_since_meal > rules->time_to_die)
	{
		if (should_die(rules))
		{
			set_death_flag(rules);
			print_death(rules, philo->id);
		}
		return (1);
	}
	return (0);
}

int	check_all_full(t_rules *rules)
{
	if (rules->must_eat_count > 0)
	{
		pthread_mutex_lock(&rules->meal_count_lock);
		if (rules->full_philos == rules->num_philos)
		{
			pthread_mutex_unlock(&rules->meal_count_lock);
			set_death_flag(rules);
			return (1);
		}
		pthread_mutex_unlock(&rules->meal_count_lock);
	}
	return (0);
}

void	*monitor(t_rules *rules, t_philo *philos)
{
	int	i;

	rules = philos[0].rules;
	while (1)
	{
		i = 0;
		while (i < rules->num_philos)
		{
			if (check_philo_death(rules, &philos[i]))
				return (NULL);
			i++;
		}
		if (check_all_full(rules))
			return (NULL);
		usleep(1000);
	}
}

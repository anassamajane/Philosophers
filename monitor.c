#include "philo.h"

int	check_philo_death(t_rules *rules, t_philo * philo)
{
	time_t	time_since_meal;

	time_since_meal = get_time() - philo->last_meal;
	if (time_since_meal > rules->time_to_die)
	{
		rules->dead_philo_id = philo->id;
		rules->sim_status = 0;
		//printf("death_time: %lld\n", get_time() - rules->start_time);
		return (1);
	}
	return (0);
}

int	check_all_full(t_rules *rules)
{
	if (rules->must_eat_count < 0)
		return (0);
	pthread_mutex_lock(&rules->meal_count_lock);
	if (rules->full_philos == rules->num_philos)
	{
		pthread_mutex_unlock(&rules->meal_count_lock);
		rules->sim_status = 0;
		return (1);
	}
	pthread_mutex_unlock(&rules->meal_count_lock);
	
}

void	*monitor(t_rules *rules, t_philo *philos)
{
	int	i;

	rules = philos[0].rules;
	while (1)
	{
		i = 0;
		if (check_all_full(rules))
			return (NULL);
		while (i < rules->num_philos)
		{
			if (check_philo_death(rules, &philos[i]))
				return (NULL);
			i++;
		}
	}
}

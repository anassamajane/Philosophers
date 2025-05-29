#include "philo_bonus.h"

int	check_philo_death(t_rules *rules, t_philo * philo)
{
	time_t	time_since_meal;

	sem_wait(rules->meal_lock);
	time_since_meal = get_time() - philo->last_meal;
	sem_post(rules->meal_lock);
	if (time_since_meal > rules->time_to_die)
	{
		sem_wait(rules->sim_lock);
		rules->dead_philo_id = philo->id;
		rules->sim_status = 0;
		sem_post(rules->sim_lock);
		return (1);
	}
	return (0);
}

int	check_all_full(t_rules *rules)
{
	if (rules->must_eat_count < 0)
		return (0);
	sem_wait(rules->meal_lock);
	if (rules->full_philos == rules->num_philos)
	{
		sem_post(rules->meal_lock);
		sem_wait(rules->sim_lock);
		rules->sim_status = 0;
		sem_post(rules->sim_lock);
		return (1);
	}
	sem_post(rules->meal_lock);
	return (0);
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

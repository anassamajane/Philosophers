#include "philo.h"

int	ft_atoi(char *str)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res);
}

int	parse_args(int ac, char **av, t_rules *rules)
{
	if (ac != 5 && ac != 6)
	{
		printf("Invalid number of arguments");
		return (1);
	}
	rules->num_philos = ft_atoi(av[1]);
	rules->time_to_die = ft_atoi(av[2]);
	rules->time_to_eat = ft_atoi(av[3]);
	rules->time_to_sleep = ft_atoi(av[4]);
	rules->must_eat_count = -1;
	if (ac == 6)
		rules->must_eat_count = ft_atoi(av[5]);
	rules->someone_died = 0;
	return (0);
}

t_philo	*init_philos(t_rules *rules)
{
	t_philo	*philos;
	int	i;

	philos = malloc(sizeof(t_philo) * (rules->num_philos));
	if (!philos)
		return (NULL);
	rules->forks = malloc(sizeof(pthread_mutex_t) * (rules->num_philos));
	if (!rules->forks)
		return (NULL);
	i = 0;
	while (i < rules->num_philos)
		pthread_mutex_init(&rules->forks[i++], NULL);
	i = 0;
	while (i < rules->num_philos)
	{
		philos[i].id = i + 1;
		philos[i].eat_count = 0;
		philos[i].last_meal = 0;
		philos[i].rules = rules;
		philos[i].left_fork = &rules->forks[i];
		philos[i].right_fork = &rules->forks[(i + 1) % rules->num_philos];
		i++;
	}
	return (philos);
}

int	main(int ac, char **av)
{
	t_rules	rules;
	t_philo	*philos;

	if (parse_args(ac, av, &rules))
		return (1);
	philos = init_philos(&rules);
	if (!philos)
		return (1);
	printf("Philo: %d\n", rules.num_philos);
	printf("Time to die: %d\n", rules.time_to_die);
	printf("Time to eat: %d\n", rules.time_to_eat);
	printf("Time to sleep: %d\n", rules.time_to_sleep);
	printf("Must eat count: %d\n", rules.must_eat_count);
}

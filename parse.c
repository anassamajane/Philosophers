#include "philo.h"

unsigned int    ft_atoi(char *str)
{
    unsigned long   res;
    int i;

    i = 0;
    res = 0;
    if (str[i] == '+')
        i++;
    while (str[i] >= '0' && str[i] <= '9')
    {
        res = res * 10 + (str[i] - '0');
        if (res >= INT_MAX)
            return (INT_MAX);
        i++;
    }
    if (str[i])
        return (INT_MAX);
    return (res);
}

int    parse_args(int ac, char **av, t_rules *rules)
{
    if (ac != 5 && ac != 6)
    {
        printf("Invalid number of arguments\n");
        return (1);
    }
    rules->num_philos = ft_atoi(av[1]);
    rules->time_to_die = ft_atoi(av[2]);
    rules->time_to_eat = ft_atoi(av[3]);
    rules->time_to_sleep = ft_atoi(av[4]);
    rules->must_eat_count = -1;
    if (ac == 6)
        rules->must_eat_count = ft_atoi(av[5]);
    rules->sim_status = 1;
    rules->dead_philo_id = -1;
    if (check_args(rules))
    {
        printf("Error parsing the arguments\n");
        return (1);
    }
    return (0);
}

int check_args(t_rules *rules)
{
    if (rules->num_philos == INT_MAX || rules->num_philos == 0 || rules->num_philos > MAX_PHILO)
        return (1);
    else if (rules->time_to_die == INT_MAX || rules->time_to_eat == INT_MAX || rules->time_to_sleep == INT_MAX || rules->must_eat_count == INT_MAX)
        return (1);
    return (0);
}

t_philo    *init_philos(t_rules *rules, t_philo *philos)
{
    int    i;

    i = 0;
    while (i < rules->num_philos)
        pthread_mutex_init(&rules->forks[i++], NULL);
    rules->full_philos = 0;
    pthread_mutex_init(&rules->meal_count_lock, NULL);
    pthread_mutex_init(&rules->sim_mutex, NULL);
    pthread_mutex_init(&rules->meal_mutex, NULL);
    i = 0;
    while (i < rules->num_philos)
    {
        philos[i].id = i + 1;
        philos[i].eat_count = 0;
        philos[i].last_meal = 0;
        philos[i].done_eating = 0;
        philos[i].rules = rules;
        philos[i].left_fork = &rules->forks[i];
        philos[i].right_fork = &rules->forks[(i + 1) % rules->num_philos];
        i++;
    }
    return (philos);
}

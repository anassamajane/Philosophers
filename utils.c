#include "philo.h"

void    smart_sleep(time_t time, t_philo *philo)
{
        time_t  start;

        start = get_time();
        while (get_sim_status(philo->rules))
        {
                if (get_time() - start >= time)
                        break;
                usleep(500);
        }
}

long long       get_time(void)
{
        struct timeval  time;
        time_t       mls;

        gettimeofday(&time, NULL);
        mls = (time.tv_sec * 1000) + (time.tv_usec / 1000);
        return (mls);
}

void    print_action(t_philo *philo, char *action)
{
        pthread_mutex_lock(&philo->rules->sim_mutex);////
        if (!philo->rules->sim_status)
        {
                pthread_mutex_unlock(&philo->rules->sim_mutex);////
                return ;
        }
        printf("%lld %d %s\n", get_time() - philo->rules->start_time, philo->id, action);
        pthread_mutex_unlock(&philo->rules->sim_mutex);////
}

void    cleaning(t_philo *philos, t_rules *rules)
{
        int     i;

        i = 0;
        while (i < rules->num_philos)
        {
                pthread_mutex_destroy(&rules->forks[i]);
		pthread_mutex_destroy(&philos[i].meal_mutex);
                i++;
        }
        pthread_mutex_destroy(&rules->meal_count_lock);
        pthread_mutex_destroy(&rules->sim_mutex);
}

int     get_sim_status(t_rules *rules)
{
        int     status;

        pthread_mutex_lock(&rules->sim_mutex);
        status = rules->sim_status;
        pthread_mutex_unlock(&rules->sim_mutex);
        return (status);
}

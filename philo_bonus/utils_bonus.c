#include "philo_bonus.h"

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
        sem_wait(philo->rules->sim_lock);
        if (!philo->rules->sim_status)
        {
                sem_post(philo->rules->sim_lock);
                return ;
        }
        sem_post(philo->rules->sim_lock);
        sem_wait(philo->rules->print);
        printf("%lld %d %s\n", get_time() - philo->rules->start_time, philo->id, action);
        sem_post(philo->rules->print);
}

// void    cleaning(t_rules *rules)
// {
//         int     i;

//         i = 0;
//         while (i < rules->num_philos)
//         {
//                 pthread_mutex_destroy(&rules->forks[i]);
//                 i++;
//         }
//         pthread_mutex_destroy(&rules->meal_count_lock);
//         pthread_mutex_destroy(&rules->sim_mutex);
//         pthread_mutex_destroy(&rules->meal_mutex);
// }

int     get_sim_status(t_rules *rules)
{
        int     status;

        sem_wait(rules->sim_lock);
        status = rules->sim_status;
        sem_post(rules->sim_lock);
        return (status);
}
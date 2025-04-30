#include "philo.h"

void    smart_sleep(long long time, t_philo *philo)
{
        long long       start;

        start = get_time();
        while (1)
        {
                pthread_mutex_lock(&philo->rules->death_check);
                if (philo->rules->someone_died)
                {
                        pthread_mutex_unlock(&philo->rules->death_check);
                        break;
                }
                pthread_mutex_unlock(&philo->rules->death_check);
                if (get_time() - start >= time)
                        break;
                usleep(500);
        }
}

long long       get_time(void)
{
        struct timeval  time;
        long long       mls;

        gettimeofday(&time, NULL);
        mls = (time.tv_sec * 1000) + (time.tv_usec / 1000);
        return (mls);
}

void    print_action(t_philo *philo, char *action)
{
        long long       timestamp;

        pthread_mutex_lock(&philo->rules->death_check);
        if (philo->rules->someone_died)
        {
                pthread_mutex_unlock(&philo->rules->death_check);
                return ;
        }
        pthread_mutex_unlock(&philo->rules->death_check);
        timestamp = get_time() - philo->rules->start_time;
        pthread_mutex_lock(&philo->rules->writing);
        printf("%lld %d %s\n", timestamp, philo->id, action);
        pthread_mutex_unlock(&philo->rules->writing);
}


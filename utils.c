#include "philo.h"

void    smart_sleep(time_t time, t_philo *philo)
{
        time_t  start;

        start = get_time();
        while (philo->rules->sim_status)
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
        time_t  timestamp;

        if (!philo->rules->sim_status || philo->done_eating)//////////
            return ;
        timestamp = get_time() - philo->rules->start_time;
        printf("%ld %d %s\n", timestamp, philo->id, action);
}

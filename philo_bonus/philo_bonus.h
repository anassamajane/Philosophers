#ifndef PHILO_BONUS_H
#define PHILO_BONUS_H

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>

# define MAX_PHILO 200


typedef struct s_rules
{
        int     num_philos;
        int     time_to_die;
        int     time_to_eat;
        int     time_to_sleep;
        int     must_eat_count;
	time_t	start_time;

        sem_t   *forks;
        sem_t   *print;
        sem_t   *death_sem;
        sem_t   *meal_lock;
	sem_t	*finished_sem;
}       t_rules;

typedef struct s_philo
{
        int     id;
        int     eat_count;
        int     done_eating;
        time_t  last_meal;
        t_rules *rules;
}	t_philo;

/* parse_bonus.c */

unsigned int    ft_atoi(char *str);
int    parse_args(int ac, char **av, t_rules *rules);
int check_args(t_rules *rules);
t_philo    *init_philos(t_rules *rules, t_philo *philos);


int init_semaphores(t_rules *rules);


/* utils_bonus.c */
void    smart_sleep(time_t time);
long long       get_time(void);
void    print_action(t_philo *philo, char *action);
int     get_sim_status(t_rules *rules);
void    cleaning(t_rules *rules);////


int	routine(t_rules *rules, t_philo *philos);
void	eat_sleep_think(t_philo *philo);


#endif

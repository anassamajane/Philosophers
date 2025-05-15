#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

# define MAX_PHILO 200

typedef struct s_rules
{
	int	num_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	must_eat_count;
	int	full_philos;
	time_t	start_time;
	int	sim_status;
	pthread_mutex_t	sim_mutex;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	meal_count_lock;
	pthread_mutex_t	forks[MAX_PHILO];
}	t_rules;

typedef struct s_philo
{
	int	id;
	int	eat_count;
	int	done_eating;
	time_t	last_meal;
	pthread_t	thread_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_rules	*rules;
}	t_philo;


/* routine.c */
void	*routine(void *args);
int     check_should_exit(t_philo *philo);
void    eat_sleep_think(t_philo *philo);
void    *one_philo_case(t_philo *philo);
void    take_forks(t_philo *philo);
void    put_down_forks(t_philo *philo);


int     create_threads(t_rules *rules, t_philo *philos);


/* parse.c */
int	parse_args(int ac, char **av, t_rules *rules);
t_philo	*init_philos(t_rules *rules, t_philo *philos);
int     ft_atoi(char *str);


/* monitor.c */
void	*monitor(t_rules *rules, t_philo *philos);
void    print_death(t_rules *rules, int id);//
void    set_death_flag(t_rules *rules);//
int     should_die(t_rules *rules);//
int     check_philo_death(t_rules *rules, t_philo * philo);//
int     check_all_full(t_rules *rules);//


/* utils.c */
void  smart_sleep(time_t time, t_philo *philo);
long long     get_time(void);
void  print_action(t_philo *philo, char *action);

#endif 

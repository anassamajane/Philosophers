#include "philo_bonus.h"

#define PHILO 5

int	main()
{
	pid_t	pids[PHILO];
	int	i;

	i = 0;
	while (i < PHILO)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			printf("Fork failed!\n");
			exit(1);
		}
		else if (pids[i] == 0)
		{
			printf("Philosopher %d started. PID: %d\n", i + 1, getpid());
			exit(0);
		}
		i++;
	}

	i = 0;
	while (i < PHILO)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
	printf("All philosphers have exited.\n");
	return (0);
}

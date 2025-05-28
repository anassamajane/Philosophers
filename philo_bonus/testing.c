#include <unistd.h>
#include <stdio.h>

/*int	main()
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		printf("I am the child process and my PID is : %d\n", getpid());
	else if (pid > 0)
		printf("Hey i am the parent process my PID is : %d and i created a child with PID : %d\n", getpid(), pid);
	else
		printf("Fork failed!\n");
	return (0);
}*/

/*int	main()
{
	pid_t	pid;
	int	x;

	pid = fork();
	x = 0;
	if (pid == 0)
	{
		x++;
		printf("Child x is: %d\n", x);
	}
	else if (pid > 0)
	{
		x--;
		printf("Parent x is: %d\n", x);
	}
	else
		printf("Fork failed!\n");
	return (0);
}*/

/*int	main()
{
	fork();
	fork();
	fork();
	printf("Hello\n");
}*/

/*int	main()
{
	pid_t	pid;
	int	x;

	pid = fork();
	x = 0;
	if (pid == 0)
	{
		printf("child x is: %d with adress: %p\n", x, &x);
	}
	else if (pid > 0)
	{
		printf("parent x is: %d with adress: %p\n", x, &x);
	}
	else
		printf("failed fork!\n");
	return (0);
}*/

/*int     main()
{
        pid_t   pid;
        int     x;

        pid = fork();
        x = 0;
        if (pid == 0)
        {
		x = 69;
                printf("child x is: %d with adress: %p\n", x, &x);
        }
        else if (pid > 0)
        {
                printf("parent x is: %d with adress: %p\n", x, &x);
        }
        else
                printf("failed fork!\n");
        return (0);
	// Same virtual address, different physical memory (after write)
}*/

/*So, even if both print the same address, like 0x7ffeabc15180, it means:

In the child, x is at that address in the child’s memory space.

In the parent, x is at that same address in the parent’s memory space.

But they are not pointing to the same physical memory anymore if one process has written to it.
*/


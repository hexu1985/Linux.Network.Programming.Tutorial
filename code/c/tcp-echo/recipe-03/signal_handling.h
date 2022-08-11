#pragma once

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

static void signal_handling(int signo)
{
    if (signo != SIGCHLD)
        return;

	pid_t	pid;
	int		stat;

	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	return;
}

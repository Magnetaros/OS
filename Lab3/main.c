#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void INT_handler(int);

static int p_pipe[2];
static int c_pipe[2];

int main(void)
{
    time_t my_time, c_time;
    pid_t pid;

    signal(SIGINT, INT_handler);

    if(pipe(p_pipe) < 0)
    {
        printf("Parent pipe failed!!!\n");
        exit(1);
    }
    if(pipe(c_pipe) < 0)
    {
        printf("Child pipe failed!!!\n");
        exit(1);
    }

    pid = fork();

    while(1)
    {
        if(pid > 0)
        {
            my_time = time(0);
            write(p_pipe[1], &my_time, sizeof(time_t));
            
            sleep(1);

            read(c_pipe[0], &c_time, sizeof(time_t));
            printf("MyTime = %ld sec, CTime = %ld sec, Latency = %ld sec\n", my_time, c_time, c_time - my_time);
        }
        else if(pid == 0)
        {
            read(p_pipe[0], &my_time, sizeof(time_t));

            sleep(1);
            c_time = time(0);
            write(c_pipe[1], &c_time, sizeof(time_t));
        }
        else
        {
            printf("Forked!!!\n");
            exit(1);
        }
    }

}

void INT_handler(int signal)
{
    printf("Closing pipes!\n");
    close(p_pipe[0]);
	close(p_pipe[1]);
	close(c_pipe[0]);
	close(c_pipe[1]);
	exit(signal);
}

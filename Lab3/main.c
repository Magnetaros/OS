#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    time_t my_time, c_time;
    int p_pipe[2];
    int c_pipe[2];
    pid_t pid;

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
            close(p_pipe[0]);
            write(p_pipe[1], &my_time, sizeof(time_t));
            close(p_pipe[1]);
            
            sleep(1);

            printf("Parent pid %d, time = %ld\n", getpid(), my_time);
            close(c_pipe[1]);
            read(c_pipe[0], &c_time, sizeof(time_t));
            printf("Parent get %ld\n", c_time);
            printf("MyTime = %ld, CTime = %ld, Latency = %d\n", my_time, c_time, (int)(c_time - my_time));
            close(c_pipe[0]);
        }
        else if(pid == 0)
        {
            c_time = time(0);
            close(p_pipe[1]);
            read(p_pipe[0], &my_time, sizeof(time_t));
            printf("Child get %ld\n", my_time);
            close(p_pipe[0]);
            close(c_pipe[0]);

            sleep(1);

            printf("Child pid %d, time = %ld\n", getpid(), c_time);
            write(c_pipe[1], &c_time, sizeof(time_t));
            close(c_pipe[1]);
        }
        else
        {
            printf("Forked!!!\n");
            exit(1);
        }
    }

}

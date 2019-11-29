#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void INT_handler(int);

static int p_pipe[2];

int main(void)
{
    char in_str[100];
    size_t buffer_size;
    time_t p_time;
    pid_t pid;

    signal(SIGINT, INT_handler);

    if(pipe(p_pipe) < 0)
    {
        printf("Parent pipe failed!!!\n");
        exit(1);
    }

    pid = fork();

    while(1)
    {
        if(pid > 0)
        {
            pid = getpid();
            p_time = time(0);

            sprintf(in_str, "pid = %d, time : %s", pid, ctime(&p_time));
            buffer_size = strlen(in_str);

            write(p_pipe[1], in_str, buffer_size * sizeof(char) + 1);
            sleep(1);
        }
        else if(pid == 0)
        {
            p_time = time(0);   
            read(p_pipe[0], in_str, 100 * sizeof(char));
            printf("Result:\n\t parent  %s", in_str);
            printf("\t child time : %s\n", ctime(&p_time));
            sleep(1);
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
	exit(signal);
}

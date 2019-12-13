#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

static int p_pipe[2];

int main()
{
    char in_str[100];
    size_t buffer_size;
    pid_t pid;

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
            time_t p_time = time(0);

            sprintf(in_str, "pid = %d, time : %s", pid, ctime(&p_time));
            buffer_size = strlen(in_str);

            write(p_pipe[1], in_str, buffer_size * sizeof(char) + 1);
            sleep(1);
        }
        else if(pid == 0)
        {
            time_t c_time = time(NULL);   
            read(p_pipe[0], in_str, 100 * sizeof(char));
            printf("Result:\n\t parent  %s", in_str);
            printf("\t child time : %s\n", ctime(&c_time));
            sleep(1);
        }
        else
        {
            printf("Forked!!!\n");
            exit(1);
        }
    }
}

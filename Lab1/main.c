#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

void at_exit_handler(void);

int main(){
    pid_t pid = fork();
    atexit(at_exit_handler);

    switch (pid)
    {
    case -1:
        perror("Forked!!!\n");
        _exit(1);
    case 0:
        printf("Child pid = %i, parent pid = %i\n", getpid(), getppid());
        break;
    default:
        waitpid(pid,0,0);
        printf("Parent pid = %i\n", getpid());
        break;
    }
    return 0;
}


void at_exit_handler(void){
    printf("atexit handler pid = %d\n", getpid());
}

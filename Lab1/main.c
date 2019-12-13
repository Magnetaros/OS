#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

void at_exit_handler(void);
void on_exit_handler(int,void*);

int main(){
    pid_t pid = fork();
    int ev = 0;
    atexit(at_exit_handler);
    on_exit(on_exit_handler, 0);

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
        printf("Parent pid = %i, exit status = %d\n", getpid(), WEXITSTATUS(ev));
        break;
    }
    return 0;
}


void at_exit_handler(void){
    printf("atexit handler pid = %d\n", getpid());
}

void on_exit_handler(int exit_status,void *arg){
    printf("on_exit handler : exit code = %d , arg = %ld\n", WEXITSTATUS(exit_status), (long)arg);
}

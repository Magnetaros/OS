#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include<sys/types.h>

void at_exit_handler(void);
void on_exit_handler(int,void*);
int atexit(void(*func)(void));
int on_exit(void(*func)(int,void*),void *arg);


int main(){
    pid_t pid = fork();
    atexit(at_exit_handler);
    on_exit(on_exit_handler,(void*)pid);
    int ev = 0;

    switch (pid)
    {
    case -1:
        perror("Forked!!!\n");
        _exit(1);
    case 0:
        printf("Child pid = %i, pid = %i\n",getpid(),pid);
        break;
    default:
        waitpid(pid,0,0);
        printf("Parent pid = %i, pid = %i, exit status = %d\n",getpid(),pid,WEXITSTATUS(ev));
        break;
    }
    return 0;
}


void at_exit_handler(void){
    printf("atexit event pid = %d\n",getpid());
}

void on_exit_handler(int exit_status,void *arg){
    printf("Process:exit code = %d , arg = %ld\n",WEXITSTATUS(exit_status),(long)arg);
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    char *path = "./myfifo";
    int r_fifo;
    int w_fifo;

    if(mkfifo(path, 0777 | O_CREAT) < 0) {
        perror("FIFO ERR: ");
        exit(-1);
    }
    else
        printf("FIFO CREATED\n");
    

    pid_t pid = fork();
    switch (pid)
    {
    case 0:
        r_fifo = open(path, O_RDONLY);
        char result_str[100];

        sleep(1);
        read(r_fifo, result_str, 100);
        printf("%s", result_str);
        printf("Child Process pid = %d\n", getpid());
        close(r_fifo);
        unlink(path);
        break;
    case -1:
        printf("Forked!!!\n");
        _exit(-1);
    default:
        w_fifo = open(path, O_WRONLY);
        char in_str[100];
        time_t p_time = time(NULL);

        sprintf(in_str," pid = %d, time : %s", getpid(), ctime(&p_time));
        write(w_fifo, in_str, 100);
        close(w_fifo);
        sleep(1);
        break;
    }
    return 0;
}

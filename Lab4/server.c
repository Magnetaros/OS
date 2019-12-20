#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#define BUFFERSIZE 1024
#define SHMEMPERM 0666

int main()
{
	pid_t pid = getpid();
	time_t timer;
	int shmem;
	char *input;
	char *path = "shmem";
	key_t key;
	int count = 100;
	struct tm *curr_time;
	char stime[10];

	key = ftok(path, 'S');
	if(key < 0)
		perror("FTOK ERR\n");

	shmem = shmget(key, BUFFERSIZE, SHMEMPERM | IPC_CREAT);
	if(shmem < 0)
		perror("SHMEM NOT FOUND\n");

	input = (char*)shmat(shmem, NULL, 0);
	printf("Wriing...\n");
	while(count)
	{
		timer = time(NULL);
		curr_time = localtime(&timer);
		strftime(stime, 20, "%H:%M:%S\n", curr_time);
		sprintf(input, "pid = %d, time : %s", pid, stime);
		sleep(1);
		count--;
	}
	sprintf(input, "END");
	printf("Stoping writing process and closing shared memory\n");
	shmdt(input);
	shmctl(shmem, IPC_RMID, NULL);

	return 0;
}


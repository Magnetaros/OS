#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#define SHMEM_BUFFER_SIZE 1024

int main()
{
	FILE *ptr = NULL;
	pid_t pid = getpid();
	time_t timer;
	int shmem;
	char *input;
	char *path = "shmem";
	key_t key;
	int count = 100;
	
	ptr = fopen(path, "w");
	fclose(ptr);

	key = ftok(path, 'S');
	if(key < 0)
		perror("FTOK ERR\n");

	shmem = shmget(key, SHMEM_BUFFER_SIZE, 0777 | IPC_CREAT | IPC_EXCL);
	if(shmem < 0)
		perror("SHMEM NOT FOUND\n");

	input = (char*)shmat(shmem, NULL, 0);
	printf("Writing...\n");
	while(count)
	{
		timer = time(NULL);
		sprintf(input, "pid = %d, time : %s", pid, ctime(&timer));
		sleep(1);
		count--;
	}
	sprintf(input, "END");
	printf("Stoping writting process\n");
	shmdt(input);

	if(remove(path) < 0)
		perror("ERR DEL FILE\n");

	return 0;
}


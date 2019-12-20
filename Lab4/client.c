#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define BUFFERSIZE 1024
#define SHMEMPERM 0666

int main(){
	key_t key;
	int shmem;
	char *path = "shmem";
	char *output;

	key = ftok(path, 'S');
	if(key < 0)
		perror("FTOK ERR\n");
	shmem = shmget(key, BUFFERSIZE, SHMEMPERM | IPC_CREAT);
	if(shmem < 0)
		perror("SHMEM NOT FOUND\n");
	
	output = (char*)shmat(shmem, NULL, 0);

	while(strcmp(output, "END") != 0)
	{
		printf("%s", output);
		sleep(1);	
	}
	printf("Closing shared memory\n");
	shmdt(output);
	shmctl(shmem, IPC_RMID, NULL);

	return 0;
}


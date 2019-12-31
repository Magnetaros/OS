#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFERSIZE 256
#define SEGMENTPERM 0666

int main()
{
    char *path = "shm";
    char *semp = "/sem";
	key_t shm_key;
	int shm_id;
	sem_t *sem;
	char *msg;

    shm_key = ftok(path, 'S');
	if(shm_key < 0) perror("FTOK SHMMEM ERR");

	shm_id = shmget(shm_key, BUFFERSIZE, SEGMENTPERM);
	if(shm_id < 0)
	{
		perror("SHMGET ERR\n");
		exit(2);
	}

	sem = sem_open(semp, 0);
	if(sem == SEM_FAILED)
	{
		perror("SEM OPEN ERR\n");
        exit(2);
	}
	
	msg = (char*)shmat(shm_id, NULL, 0);
	while(strcmp(msg, "END") != 0)
	{
        sem_wait(sem);
		printf("%s", msg);
		sem_post(sem);
        sleep(1);
	}
	sem_close(sem);
	shmdt(msg);

    return 0;
}

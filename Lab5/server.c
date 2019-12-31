#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#define BUFFERSIZE 256
#define SEGMENTPERM 0666

int main()
{
	char *path = "shm";
	char *semp = "sem";
	key_t shm_key;
	int shm_id;
	sem_t *sem;
	char *msg;

	shm_key = ftok(path, 'S');
	if(shm_key < 0) perror("FTOK SHMMEM ERR");

	shm_id = shmget(shm_key, BUFFERSIZE, SEGMENTPERM | IPC_CREAT | IPC_EXCL);
	if(shm_id < 0)
	{
		perror("SHMGET ERR");
		if(errno == EEXIST)
			shm_id = shmget(shm_key, BUFFERSIZE, SEGMENTPERM);
		else
			exit(EXIT_FAILURE);
	}

	sem = sem_open(semp, O_CREAT, SEGMENTPERM, 'a');
	if(sem == SEM_FAILED)
	{
		perror("SEM OPEN ERR");
		if(errno == EEXIST)
			sem = sem_open(semp, O_CREAT);
		else
		{
			perror("");
			exit(EXIT_FAILURE);
		}
			
	}
	
	msg = (char*)shmat(shm_id, NULL, 0);
	int count = 100;
	pid_t pid = getpid();
	time_t timer;
	printf("Writing...\n");
	while(count)
	{
		sleep(1);
		sem_wait(sem);
		timer = time(NULL);
		sprintf(msg, "pid : %d, %s", pid, ctime(&timer));
		sem_post(sem);
		count--;
	}
	sprintf(msg, "END");
	printf("Stoping writing process\n");
	sem_close(sem);
	sem_unlink(semp);
	shmdt(msg);
	shmctl(shm_id, IPC_RMID, NULL);
	return 0;
}

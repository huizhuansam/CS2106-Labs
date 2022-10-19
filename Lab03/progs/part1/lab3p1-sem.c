#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>

#define NUM_PROCESSES 5

int main() {
  int i, j, pid, shmid;
  sem_t *semaphores;

  // Create shared memory for 5 semaphores
  shmid = shmget(IPC_PRIVATE, NUM_PROCESSES * sizeof(sem_t), IPC_CREAT | 0600);

  if (shmid == -1) {
    printf("Unable to create shared memory region.");
    exit(1);
  }

  semaphores = (sem_t*) shmat(shmid, NULL, 0);

  if (semaphores == (sem_t*) -1) {
    printf("Cannot attach shared memory.\n");
    exit(1);
  }

  for (i = 0; i < NUM_PROCESSES; i++) {
    sem_init(&semaphores[i], 1, 0);
    if ((pid = fork()) == 0) {
      break;
    }
  }

  if (pid == 0) {
    sem_wait(&semaphores[i]);
    printf("I am child %d\n", i);
    for (j = i * 10; j < i * 10 + 10; j++) {
      printf("%d ", j);
      fflush(stdout);
      usleep(250000);
    }
    printf("\n\n");
    sem_post(&semaphores[i]);
    sem_destroy(&semaphores[i]);
    if (i < NUM_PROCESSES - 1) {
      sem_post(&semaphores[i + 1]);
    }
  } else {
    sem_post(&semaphores[0]);
    for (i = 0; i < NUM_PROCESSES; i++) {
      wait(NULL);
    }
    shmdt((char*) semaphores);
    shmctl(shmid, IPC_RMID, 0);
  }
}


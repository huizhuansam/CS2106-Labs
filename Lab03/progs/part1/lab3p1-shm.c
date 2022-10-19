#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#define NUM_PROCESSES 5

int main() {
  int i, j, pid;
  int *lock;
  int shmid;
  shmid = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | 0600);

  if (shmid == -1) {
    printf("Unable to create shared memory region.\n");
    exit(1);
  }

  lock = (int*) shmat(shmid, NULL, 0);

  if (lock == (int*) -1) {
    printf("Cannot attach shared memory.\n");
    exit(1);
  }

  lock[0] = 1;
  lock[1] = 0;

  for (i = 0; i < NUM_PROCESSES; i++) {
    if ((pid = fork()) == 0) {
      break;
    }
  }
  if (pid == 0) {
    while (lock[0] == 0 || lock[1] != i);

    lock[0] = 0;
    printf("I am child %d\n", i);
    for (j = i * 10; j < i * 10 + 10; j++){
      printf("%d ", j);
      fflush(stdout);
      usleep(250000);
    }
    printf("\n\n");
    lock[0] = 1;
    lock[1]++;
  } else {
    for (i = 0; i < NUM_PROCESSES; i++) {
      wait(NULL);
    }

    shmdt(lock);
    shmctl(shmid, IPC_RMID, 0);
  }
}

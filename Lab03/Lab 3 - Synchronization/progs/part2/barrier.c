#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

int nproc, barrier_shmid, count_mutex_shmid, count_shmid, *count;
sem_t *barrier, *count_mutex;

void init_barrier(int numproc) {
  nproc = numproc;

  // Create shared memory regions
  barrier_shmid = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
  count_mutex_shmid = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
  count_shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);

  if (barrier_shmid == -1 || count_mutex_shmid == -1 || count_shmid == -1) {
    printf("Cannot create shared memory!\n");
    exit(1);
  }

  // Attach shared memory regions
  barrier = (sem_t*) shmat(barrier_shmid, NULL, 0);
  count_mutex = (sem_t*) shmat(count_mutex_shmid, NULL, 0);
  count = (int*) shmat(count_shmid, NULL, 0);

  if (barrier == (sem_t*) -1 || count_mutex == (sem_t*) -1 || count == (int*) -1) {
    printf("Cannot attach shared memory!\n");
    exit(1);
  }

  sem_init(barrier, 1, 0);
  sem_init(count_mutex, 1, 1);
}

void reach_barrier() {
  sem_wait(count_mutex);
  (*count)++;
  sem_post(count_mutex);
  if (*count == nproc) {
    sem_post(barrier);
  } else {
    sem_wait(barrier);
    sem_post(barrier);
  }
}

void destroy_barrier(int my_pid) {
  if (my_pid != 0) {
    // Destroy semaphores
    sem_destroy(barrier);
    sem_destroy(count_mutex);
    // Detach shared memory regions
    shmdt((char*) barrier);
    shmdt((char*) count_mutex);
    shmdt((char*) count);
    // Destroy shared memory regions
    shmctl(barrier_shmid, IPC_RMID, 0);
    shmctl(count_shmid, IPC_RMID, 0);
    shmctl(count_mutex_shmid, IPC_RMID, 0);
  }
}


#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#define CHILD_PROCESS 0

int main() {
  if(fork() ==  CHILD_PROCESS) {
    char *args[] = {"cat", "file.txt", NULL};
    execvp("cat", args);
  } else {
    wait(NULL);
  }
}


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#define CHILD 0

int main(void) {
  printf("Be patient, the program will take around 7 seconds to run.\n");
  printf("At the end you can do \"cat results.out\" to see the result.\n");

  int p[2];
  int fp_out = open("./results.out", O_CREAT | O_WRONLY);

  if (pipe(p) < 0) {
    perror("lab2p2e: ");
  }

  if (fork() != CHILD) { 
    // Close reading end
    close(p[0]);    
    // Output from STDOUT will be directed into write end of the pipe
    dup2(p[1], STDOUT_FILENO);     
    execlp("./slow", "slow", "5", NULL); 
    // Done writing, close writing end
    close(p[1]); 
  } else {
    // Close writing end
    close(p[1]);
    // Direct bytes from pipe to STDIN
    dup2(p[0], STDIN_FILENO);
    // Direct STDOUT to output file
    dup2(fp_out, STDOUT_FILENO);
    execlp("./talk", "talk", NULL);
    // Close reading end
    close(p[0]);
    // Close output file
    close(fp_out);
  }

  return 0;
}


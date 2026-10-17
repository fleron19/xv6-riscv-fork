#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/types.h"

void printargs_nomut(int pid, int argc, char **argv){
    for (int argn = 0; argn < argc; argn ++) {
        for (int charn = 0; charn < strlen(argv[argn]); charn++) {
            fprintf(2, "%d: arg %d, char  '%c'\n", pid, argn, argv[argn][charn]);
        }
    }
}

void printargs_mut(int pid, int argc, char **argv){ // mutex
    for (int argn = 0; argn < argc; argn ++) {
        for (int charn = 0; charn < strlen(argv[argn]); charn++) {
            fprintf(2, "%d: arg %d, char  '%c'\n", pid, argn, argv[argn][charn]);
        }
    }
}

int
main(int argc, char **argv)
{
  fprintf(2, "------without mutex------\n");

  int pid1 = fork();
  if (pid1 < 0) {
    fprintf(2, "Fork 1 error\n");
    exit(1);
  }

  if (pid1 == 0) {
    printargs_nomut(getpid(), argc, argv);
    exit(0);
  }

  int pid2 = fork();
  if (pid2 < 0) {
    fprintf(2, "Fork 2 error\n");
    exit(1);
  }

  if (pid2 == 0) {
    printargs_nomut(getpid(), argc, argv);
    exit(0);
  }

  wait(0);
  wait(0);

  fprintf(2, "----------mutex----------\n");
  
  // mutex

  int pid3 = fork();
  if (pid3 < 0) {
    fprintf(2, "Fork 3 error\n");
    exit(1);
  }

  if (pid3 == 0) {
    printargs_mut(getpid(), argc, argv); // mutex
    exit(0);
  }

  int pid4 = fork();
  if (pid4 < 0) {
    fprintf(2, "Fork 4 error\n");
    exit(1);
  }

  if (pid4 == 0) {
    printargs_mut(getpid(), argc, argv); // mutex 
    exit(0);
  }

  wait(0);
  wait(0);
  exit(0);
}

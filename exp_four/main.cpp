#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
using std::cin;
using std::cout;
using std::endl;
const int ChildCount = 2;

pid_t pid1;
pid_t pid2;

void parent_sigint_handler(int) {
  kill(pid1, SIGINT);
  kill(pid2, SIGINT);
}

void child1_sigint_handler(int) {
  cout << "Child Process 1 is killed by Parent!" << endl;
  exit(0);
}

void child2_sigint_handler(int) {
  cout << "Child Process 2 is killed by Parent!" << endl;
  exit(0);
}

int main(void) {
  int pipefd[2]; // 0 for read, 1 for write
  pipe(pipefd);
  pid1 = fork();
  if (0 == pid1) {
    // child 1
    signal(SIGINT, child1_sigint_handler);
    close(pipefd[0]);
    int times = 1;
    while (1) {
      char buff[256];
      sprintf(buff, "I send message %d times", times);
      write(pipefd[1], buff, strlen(buff));
      times++;
      sleep(1);
    }
  } else {
    pid2 = fork();
    if (0 == pid2) {
      // child 2
      signal(SIGINT, child2_sigint_handler);
      close(pipefd[1]);
      while (1) {
        char buff[256];
        read(pipefd[0], buff, 256);
        cout << buff << endl;
      }
    } else {
      signal(SIGINT, parent_sigint_handler);
    }
  }
  while (1) {
    wait(NULL);
    close(pipefd[0]);
    close(pipefd[1]);
    cout << "Parent Process is Killed!" << endl;
    exit(0);
  }
}

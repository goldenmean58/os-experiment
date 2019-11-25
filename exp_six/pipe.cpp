#include "main.h"

int pipe(void) {
  int pipefd[2];
  pipe(pipefd);
  pid_t pid = fork();
  if (0 != pid) {
    close(pipefd[0]);
    char buff[256];
    sprintf(buff, "Child(PID=%d) is sending a message to parent!", pid);
    write(pipefd[1], buff, strlen(buff));
  } else {
    close(pipefd[1]);
    char buff[256];
    read(pipefd[0], buff, 256);
    cout << "Parent received a message from child: " << buff << endl;
  }
  return 0;
}

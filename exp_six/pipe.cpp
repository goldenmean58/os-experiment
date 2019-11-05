#include <iostream>
#include <string.h>
#include <unistd.h>
using std::cin;
using std::cout;
using std::endl;

int main(void) {
  int pipefd[2];
  pipe(pipefd);
  pid_t pid = fork();
  if (0 == pid) {
    close(pipefd[0]);
    char buff[] = "Child is sending a message to parent!";
    write(pipefd[1], buff, strlen(buff));
  } else {
    close(pipefd[1]);
    char buff[256];
    read(pipefd[0], buff, 256);
    cout << buff << endl;
  }
  return 0;
}

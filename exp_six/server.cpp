#include <iostream>
#include <sys/msg.h>
#include <unistd.h>
using std::cin;
using std::cout;
using std::endl;

struct msgbuff {
  long mtype;
  pid_t pid;
};

int main(void) {
  key_t key = 1234;
  int msqid = msgget(key, IPC_CREAT | 0666);
  struct msgbuff mbuf;
  mbuf.mtype = 1;
  msgrcv(msqid, &mbuf, sizeof(pid_t), 0, MSG_NOERROR);
  cout << "Serving for client " << mbuf.pid << endl;
  mbuf.mtype = mbuf.pid;
  mbuf.pid = getpid();
  msgsnd(msqid, &mbuf, sizeof(pid_t), MSG_NOERROR);
  return 0;
}

#include "main.h"

int server(void) {
  int sem1 = init_sem(IPC_CREAT);
  key_t key = 1234;
  int msqid = msgget(key, IPC_CREAT | 0666);
  struct msgbuff mbuf;
  mbuf.mtype = 1;
  p(sem1);
  msgrcv(msqid, &mbuf, sizeof(pid_t), 0, MSG_NOERROR);
  cout << "Server: Serving for client PID: " << mbuf.pid << endl;
  mbuf.mtype = mbuf.pid;
  mbuf.pid = getpid();
  msgsnd(msqid, &mbuf, sizeof(pid_t), MSG_NOERROR);
  return 0;
}

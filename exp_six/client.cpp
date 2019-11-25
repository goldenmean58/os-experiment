#include "main.h"

int client(void) {
  int sem1 = init_sem(IPC_CREAT);
  key_t key = 1234;
  int msqid = msgget(key, IPC_CREAT | 0666);
  struct msgbuff mbuf;
  mbuf.mtype = 1;
  mbuf.pid = getpid();
  v(sem1);
  msgsnd(msqid, &mbuf, sizeof(pid_t), MSG_NOERROR);
  msgrcv(msqid, &mbuf, sizeof(pid_t), 0, MSG_NOERROR);
  cout << "Client: Receive reply from PID: " << mbuf.pid << endl;
  return 0;
}

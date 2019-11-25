#include "main.h"
extern int errno;

int init_sem(key_t key) {
  int nsems = 1;
  int semflg = IPC_CREAT | 0666;
  int semaphore = semget(key, nsems, semflg);
  if (semaphore == -1) {
    perror("Failed to get a semaphore: ");
    exit(-1);
  }
  union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
  } arg;
  arg.val = 0;
  int ret = semctl(semaphore, 0, SETVAL, arg);
  if (ret == -1) {
    perror("Failed to intialize the semaphore: ");
    exit(-1);
  }
  return semaphore;
}

void p(int semaphore) {
  sembuf sops;
  sops.sem_flg = SEM_UNDO;
  sops.sem_op = 1;
  sops.sem_num = 0;
  semop(semaphore, &sops, 1);
}

void v(int semaphore) {
  sembuf sops;
  sops.sem_flg = SEM_UNDO;
  sops.sem_op = -1;
  sops.sem_num = 0;
  semop(semaphore, &sops, 1);
}

int main(void) {
  int choice = 0;
  pid_t pid = 0;
  cout << "1. Pipe" << endl
       << "2. Msg buffer queue" << endl
       << "3. Shared buffer" << endl
       << "Choice: ";
  cin >> choice;
  switch (choice) {
  case 1:
    pipe();
    break;
  case 2:
    pid = fork();
    if (pid == 0) {
      server();
    } else {
      client();
    }
    break;
  case 3:
    pid = fork();
    if (pid == 0) {
      a();
    } else {
      b();
    }
    break;
  default:
    cout << "Bad input!" << endl;
    return -1;
    break;
  }
  return 0;
}

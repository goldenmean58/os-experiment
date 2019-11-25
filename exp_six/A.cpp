#include "main.h"

int a(void) {
  int sem1 = init_sem(IPC_CREAT);
  int sem2 = init_sem(IPC_CREAT + 1);
  key_t key = 3523;
  int shm_id = shmget(key, 512, IPC_CREAT | 0666);
  if (-1 == shm_id) {
    cout << "Failed to get the shared memory" << endl;
    return -1;
  }
  char buff[] = "Hello world!";
  char *addr = static_cast<char *>(shmat(shm_id, NULL, 0));
  if (addr == (char *)-1) {
    cout << "Failed" << endl;
    return 0;
  }
  strcpy(addr, buff);
  cout << "A write: " << buff << endl;
  p(sem1);
  v(sem2);
  cout << "A read: " << addr << endl;
  shmdt(addr);
  return 0;
}

#include "main.h"

int b(void) {
  int sem1 = init_sem(IPC_CREAT);
  int sem2 = init_sem(IPC_CREAT + 1);
  key_t key = 3523;
  int shm_id = shmget(key, 512, IPC_CREAT | 0666);
  if (-1 == shm_id) {
    cout << "Failed to get the shared memory" << endl;
    return -1;
  }
  char buff[] = "Hello Unix!";
  v(sem1);
  char *addr = static_cast<char *>(shmat(shm_id, NULL, 0));
  cout << "B read: " << addr << endl;
  strcpy(addr, buff);
  cout << "B write: " << addr << endl;
  p(sem2);
  shmdt(addr);
  return 0;
}

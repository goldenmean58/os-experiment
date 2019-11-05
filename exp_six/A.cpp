#include <iostream>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>
using std::cin;
using std::cout;
using std::endl;

int main(void) {
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
  cout << buff << endl;
  while (1) {
    if (0 != strcmp(addr, buff)) {
      cout << addr << endl;
      shmdt(addr);
      return 0;
    }
  }
  return 0;
}

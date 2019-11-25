#include <error.h>
#include <iostream>
#include <string.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
using std::cin;
using std::cout;
using std::endl;
int a(void);
int b(void);
int server(void);
int client(void);
int pipe(void);
int init_sem(key_t);
void p(int);
void v(int);

struct msgbuff {
  long mtype;
  pid_t pid;
};

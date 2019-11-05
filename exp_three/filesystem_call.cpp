#include <error.h>
#include <fcntl.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
extern int errno;
using std::cin;
using std::cout;
using std::endl;
using std::string;

const int EntryCount = 6;
const char *MenuChoiceStr[EntryCount] = {"1.Create a new file",
                                         "2.Write a file",
                                         "3.Read a file",
                                         "4.Modify the permissions of a file",
                                         "5.Query the permissions of the file",
                                         "6.Exit"};

int get_choice() {
  int choice = 0;
  cout << "Menu:" << endl;
  for (int i = 0; i < EntryCount; i++) {
    cout << MenuChoiceStr[i] << endl;
  }
  cout << "Choice: ";
  cin >> choice;
  return choice;
}

void print_err() {
  strerror(errno);
  perror("Error");
}

int create_file(string &filename) {
  int fd = creat(filename.c_str(), S_IRUSR | S_IWUSR | S_IRGRP);
  if (-1 == fd) {
    cout << "Failed to create the file " << filename << endl;
    print_err();
    return -1;
  }
  close(fd);
  return 0;
}

int write_file(string &filename, string buff) {
  int fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC,
                S_IRUSR | S_IWUSR | S_IRGRP);
  if (-1 == fd) {
    cout << "Failed to open the file " << filename << endl;
    print_err();
    return -1;
  }
  size_t size_to_write = buff.length();
  ssize_t size_written = write(fd, (void *)buff.c_str(), size_to_write);
  close(fd);
  return 0;
}

size_t get_file_size(string &filename) {
  struct stat buf;
  if (-1 == stat(filename.c_str(), &buf)) {
    cout << "Failed to get information of the file " << filename;
    print_err();
    return -1;
  }
  return buf.st_size;
}

int read_file(string &filename) {
  int fd = open(filename.c_str(), O_RDONLY);
  if (-1 == fd) {
    cout << "Failed to read the file " << filename << endl;
    print_err();
    return -1;
  }
  size_t size_to_read = get_file_size(filename);
  if (-1 == size_to_read)
    return -1;
  char *buf = new char[size_to_read];
  ssize_t size_read = read(fd, static_cast<void *>(buf), size_to_read);
  close(fd);
  cout << buf << endl;
  delete[] buf;
  return 0;
}

int change_file_mode(string &filename, __mode_t mode) {
  if (-1 == chmod(filename.c_str(), mode)) {
    cout << "Failed to change mode of the file " << filename << endl;
    print_err();
    return -1;
  }
  return 0;
}

int get_mode(string &filename) {
  struct stat buf;
  if (-1 == stat(filename.c_str(), &buf)) {
    cout << "Failed to get information of the file " << filename;
    print_err();
    return -1;
  }
  return buf.st_mode;
}

int bmode2dmode(__mode_t mode) {
  int bits[3];
  int res = 0;
  for (int i = 0; i < 3; ++i) {
    bits[i] = mode & 7;
    res += bits[i] * pow(10, i);
    mode >>= 3;
  }
  return res;
}

__mode_t dmode2bmode(int mode) {
  __mode_t res = 0;
  int bits[3];
  for (int i = 0; i < 3; ++i) {
    bits[i] = mode / static_cast<int>(pow(10.0, static_cast<double>(i))) % 10;
    res |= bits[i] << i * 3;
  }
  return res;
}

void controller(int choice) {
  if (choice < 1 || choice > EntryCount) {
    cout << "Invalid choice!" << endl;
    return;
  }
  string filename;
  string buff;
  int mode;
  switch (choice) {
  case 6:
    exit(0);
    break;
  case 1:
    cout << "filename: ";
    cin >> filename;
    if (0 == create_file(filename))
      cout << "Created the file " << filename << " successfully!" << endl;
    break;
  case 2:
    cout << "filename: ";
    cin >> filename;
    cout << "content:" << endl;
    cin >> buff;
    if (0 == write_file(filename, buff))
      cout << "Write the file " << filename << " successfully!" << endl;
    break;
  case 3:
    cout << "filename: ";
    cin >> filename;
    cout << "content:" << endl;
    read_file(filename);
    break;
  case 4:
    cout << "filename: ";
    cin >> filename;
    cout << "privilege mode: ";
    cin >> mode;
    change_file_mode(filename, dmode2bmode(mode));
    break;
  case 5:
    cout << "filename: ";
    cin >> filename;
    cout << "privilege mode: " << bmode2dmode(get_mode(filename)) << endl;
    break;
  }
}

int main(void) {
  while (1) {
    int choice = get_choice();
    controller(choice);
  }
  return 0;
}

#include <algorithm>
#include <array>
#include <deque>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
using std::cin;
using std::cout;
using std::endl;
#define _K(x) (x * 1024)

constexpr int MemorySize = _K(32);

constexpr int PageSize = _K(1);

constexpr int PageFrameCount = 29;

class Scheduler {
protected:
  std::deque<int> pages;
  std::array<int, PageFrameCount> t;
  int page_lack_time = 0;
  int address2page(int address) { return address % (PageSize - 1); }
  virtual std::deque<int>::iterator choose_page_out() = 0;

public:
  int visit(int address) {
    if (address < 0 || address > MemorySize - 1) {
      cout << "Address " << address << " is out of memory bound!" << endl;
      return 1; // Out of memory bound
    }
    int page_index = this->address2page(address);
    cout << "Visit Page " << page_index << endl;
    std::vector<int> s;
    for (std::size_t i = 0; i < pages.size(); ++i) {
      t[i]++;
    }
    if (pages.end() ==
        std::find(std::begin(pages), std::end(pages), page_index)) {
      cout << "Page lack!" << endl;
      this->page_lack_time++;
      if (pages.size() >= PageFrameCount) {
        std::deque<int>::iterator page_out = this->choose_page_out();
        int index = std::distance(pages.begin(), page_out);
        cout << index << endl;
        t[index] = 0;
        cout << "eliminate page " << index << endl;
        pages.erase(page_out);
      }
      pages.push_back(page_index);
    }
    return 0;
  }
  void display() {
    cout << "Page lack time = " << this->page_lack_time
         << ", Hit rate = " << 1 - this->page_lack_time / 320.0 << endl;
  }
  Scheduler() { t.fill(0); }
};

class SchedulerFIFO : public Scheduler {
  std::deque<int>::iterator choose_page_out() final override {
    cout << "S1" << endl;
    return this->pages.begin();
  }
};

class SchedulerLRU : public Scheduler {
  std::deque<int>::iterator choose_page_out() final override {
    cout << "S2" << endl;
    int max_i = 0, max_t = t[0];
    std::deque<int>::iterator it = this->pages.begin();
    for (std::size_t i = 0; i < t.size(); ++i) {
      if (t[i] > max_t) {
        max_i = i;
        max_t = t[i];
      }
    }
    for (int i = 0; i < max_i; ++i)
      it++;
    return it;
  }

public:
};

int random_integer_in_range(int a, int b) {
  // Return a random integer in [a, b]
  double percent = static_cast<double>(rand()) / RAND_MAX; // [0, RAND_MAX]
  return a + percent * (b - a) + 0.5;
}

int main(void) {
  srand(static_cast<unsigned int>(time(NULL)));
  SchedulerFIFO s1;
  SchedulerLRU s2;
  int instruction_count = 0;
  int r = 0;
  while (true) {
    r = random_integer_in_range(1, _K(32) - 2);
    s1.visit(r);
    s2.visit(r);
    instruction_count += 2;
    s1.visit(r + 1);
    s2.visit(r + 1);
    instruction_count += 2;
    r = random_integer_in_range(0, r - 1);
    s1.visit(r);
    s2.visit(r);
    instruction_count += 2;
    s1.visit(r + 1);
    s2.visit(r + 1);
    instruction_count += 2;
    r = random_integer_in_range(r + 2, _K(32) - 2);
    s1.visit(r);
    s2.visit(r);
    instruction_count += 2;
    if (instruction_count >= 320)
      break;
    s1.visit(r + 1);
    s2.visit(r + 1);
    instruction_count += 2;
  }
  for (int i = 0; i < 320; i++) {
  }
  s1.display();
  s2.display();
  return 0;
}

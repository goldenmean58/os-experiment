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

int address2page(int address) { return address % (PageSize - 1); }

class Scheduler {
protected:
  std::deque<int> pages;
  std::array<int, PageFrameCount> t;
  int page_lack_time = 0;
  int instruction_count = 0;
  virtual std::deque<int>::iterator choose_page_out() = 0;

public:
  int visit(int address) {
    if (address < 0 || address > MemorySize - 1) {
      cout << "Address " << address << " is out of memory bound!" << endl;
      return 1; // Out of memory bound
    }
    int page_index = address2page(address);
    // cout << "Visit Page " << page_index << endl;
    std::vector<int> s;
    for (std::size_t i = 0; i < pages.size(); ++i) {
      t[i]++;
    }
    if (pages.end() ==
        std::find(std::begin(pages), std::end(pages), page_index)) {
      // cout << "Page lack!" << endl;
      this->page_lack_time++;
      if (pages.size() >= PageFrameCount) {
        std::deque<int>::iterator page_out = this->choose_page_out();
        int index = std::distance(pages.begin(), page_out);
        // cout << index << endl;
        t[index] = 0;
        // cout << "eliminate page " << index << endl;
        pages.erase(page_out);
      }
      pages.push_back(page_index);
    }
    this->instruction_count++;
    return 0;
  }
  void display() {
    cout << "Page lack time = " << this->page_lack_time
         << ", Hit rate = " << 1 - this->page_lack_time / 320.0;
  }
  Scheduler() { t.fill(0); }
};

class SchedulerFIFO : public Scheduler {
  std::deque<int>::iterator choose_page_out() final override {
    // cout << "S1" << endl;
    return this->pages.begin();
  }
};

class SchedulerLRU : public Scheduler {
  std::deque<int>::iterator choose_page_out() final override {
    // cout << "S2" << endl;
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

class SchedulerOptimal : public Scheduler {
  std::vector<int> visit_page_index;
  std::deque<int>::iterator choose_page_out() final override {
    // cout << "S3" << endl;
    std::array<int, PageFrameCount> next_step_to_visit = {0};
    std::deque<int>::iterator it = this->pages.begin();
    std::array<int, PageFrameCount>::iterator step = next_step_to_visit.begin();
    for (; it != pages.end() && step != next_step_to_visit.end();
         ++it, ++step) {
      *step = 319 - this->instruction_count;
      for (int i = this->instruction_count; i < 320; ++i) {
        if (this->visit_page_index[i] == *it) {
          *step = i - this->instruction_count;
          break;
        }
      }
    }
    auto max_ele =
        std::max_element(next_step_to_visit.begin(), next_step_to_visit.end());
    int index = std::distance(next_step_to_visit.begin(), max_ele);
    return this->pages.begin() + index;
  }

public:
  SchedulerOptimal(std::vector<int> visit_address)
      : visit_page_index(visit_address) {
    std::transform(this->visit_page_index.begin(), this->visit_page_index.end(),
                   this->visit_page_index.begin(), address2page);
  }
};

int random_integer_in_range(int a, int b) {
  // Return a random integer in [a, b]
  double percent = static_cast<double>(rand()) / RAND_MAX; // [0, RAND_MAX]
  return a + percent * (b - a) + 0.5;
}

std::vector<int> make_visit() {
  int instruction_count = 0;
  int r = 0;
  std::vector<int> visit_address;
  while (true) {
    r = random_integer_in_range(1, _K(32) - 2);
    visit_address.push_back(r);
    instruction_count += 2;
    visit_address.push_back(r + 1);
    instruction_count += 2;
    r = random_integer_in_range(0, r - 1);
    visit_address.push_back(r);
    instruction_count += 2;
    visit_address.push_back(r + 1);
    instruction_count += 2;
    r = random_integer_in_range(r + 2, _K(32) - 2);
    visit_address.push_back(r);
    instruction_count += 2;
    if (instruction_count >= 320)
      break;
    visit_address.push_back(r + 1);
    instruction_count += 2;
  }
  return visit_address;
}

int main(void) {
  srand(static_cast<unsigned int>(time(NULL)));
  SchedulerFIFO s1;
  SchedulerLRU s2;
  std::vector<int> visit_address = make_visit();
  for (size_t i = 0; i < visit_address.size(); ++i) {
    s1.visit(visit_address[i]);
    s2.visit(visit_address[i]);
  }
  SchedulerOptimal s3(visit_address);
  for (auto address : visit_address) {
    s3.visit(address);
  }
  cout << "FIFO: ";
  s1.display();
  cout << endl << "LRU: ";
  s2.display();
  cout << endl << "Optimal: ";
  s3.display();
  cout << endl;
  return 0;
}

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
using std::array;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Operation {
public:
  string name;
  float arrive_time;
  float time_cost;

  float wait_time;
  float finish_time;
  float react_rate;      // wait_time / time_cost + 1
  float cycle_time;      // finish_time - arrive_time
  float rate_cycle_time; // cycle_time / time_cost

  bool is_done;
  bool is_arrived;
  Operation() : wait_time(0), is_done(false), is_arrived(false) {}
  Operation(string &&name, float arrive_time, float time_cost)
      : name(name), arrive_time(arrive_time), time_cost(time_cost),
        is_done(false), is_arrived(false) {}
  void set_basic_data(string name, float arrive_time, float time_cost) {
    this->name = name;
    this->arrive_time = arrive_time;
    this->time_cost = time_cost;
  }
};

class Operator {
public:
  float finish_time;
  Operator() : finish_time(0) {}
  void pick_and_do_operation(int current_time,
                             std::vector<Operation>::iterator op) {
    finish_time = current_time + op->time_cost;
    op->finish_time = finish_time;
    op->wait_time = current_time - op->arrive_time;
    op->react_rate = op->wait_time / op->time_cost + 1;
    op->cycle_time = op->finish_time - op->arrive_time;
    op->rate_cycle_time = op->cycle_time / op->time_cost;
    op->is_done = true;
  }
};

auto get_ave_time(
    array<Operation, 10> ops,
    std::function<bool(const Operation &, const Operation &)> sort_function) {
  Operator optors[2];
  Operator *optor = NULL;
  vector<Operation> arrived_operations;
  float current_time = 0;
  float max_current_time = 0;
  float average_rate_cycle_time = 0, average_cycle_time = 0;
  float count_rate_cycle_time = 0, count_cycle_time = 0;
  cout << "Order: ";
  for (int i = 0; i < 10; i++) {
    for (auto &op : ops) {
      if (op.is_arrived)
        continue;
      if (op.arrive_time <= current_time) {
        arrived_operations.push_back(op);
        op.is_arrived = true;
      } else if (arrived_operations.empty() &&
                 op.arrive_time <= max_current_time) {
        arrived_operations.push_back(op);
        op.is_arrived = true;
        break;
      }
    }

    for (auto &op : arrived_operations) {
      op.wait_time = current_time - op.arrive_time;
    }

    std::vector<Operation>::iterator first_ele = std::min_element(
        arrived_operations.begin(), arrived_operations.end(), sort_function);
    cout << first_ele->name << " ";

    optor = optors[0].finish_time <= optors[1].finish_time ? &optors[0]
                                                           : &optors[1];
    current_time = first_ele->arrive_time > optor->finish_time
                       ? first_ele->arrive_time
                       : optor->finish_time;

    optor->pick_and_do_operation(current_time, first_ele);

    current_time = optors[0].finish_time > optors[1].finish_time
                       ? optors[1].finish_time
                       : optors[0].finish_time;
    max_current_time = optors[0].finish_time <= optors[1].finish_time
                           ? optors[1].finish_time
                           : optors[0].finish_time;

    count_rate_cycle_time += first_ele->rate_cycle_time;
    count_cycle_time += first_ele->cycle_time;

    arrived_operations.erase(first_ele);
  }
  cout << endl;
  average_rate_cycle_time = count_rate_cycle_time / ops.size();
  average_cycle_time = count_cycle_time / ops.size();
  return std::make_tuple(average_cycle_time, average_rate_cycle_time);
}

void do_operations(
    array<Operation, 10> &ops,
    std::function<bool(const Operation &, const Operation &)> sort_function) {
  for (Operation &op : ops) {
    op.is_arrived = false;
    op.is_done = false;
  }
  std::sort(ops.begin(), ops.end(),
            [](const Operation &op1, const Operation &op2) {
              return op1.arrive_time < op2.arrive_time;
            });
  auto [average_cycle_time, average_rate_cycle_time] =
      get_ave_time(ops, sort_function);
  cout << "average_cycle_time: " << average_cycle_time << endl
       << "average_rate_cycle_time: " << average_rate_cycle_time << endl;
}

int main() {
  array<Operation, 10> ops;
  ops[0].set_basic_data("A", 0, 7);
  ops[1].set_basic_data("B", 2, 10);
  ops[2].set_basic_data("C", 5, 20);
  ops[3].set_basic_data("D", 7, 30);
  ops[4].set_basic_data("E", 12, 40);
  ops[5].set_basic_data("F", 15, 8);
  ops[6].set_basic_data("G", 4, 8);
  ops[7].set_basic_data("H", 6, 20);
  ops[8].set_basic_data("I", 8, 10);
  ops[9].set_basic_data("J", 10, 12);

  cout << "First in first server:" << endl;
  do_operations(ops, [](const Operation &op1, const Operation &op2) -> bool {
    return op1.arrive_time < op2.arrive_time;
  });
  cout << endl;

  cout << "Short operation first:" << endl;
  do_operations(ops, [](const Operation &op1, const Operation &op2) -> bool {
    return op1.time_cost < op2.time_cost;
  });
  cout << endl;

  cout << "High react rate first:" << endl;
  do_operations(ops, [](const Operation &op1, const Operation &op2) -> bool {
    return (op1.wait_time / op1.time_cost) > (op2.wait_time / op2.time_cost);
  });
  return 0;
}

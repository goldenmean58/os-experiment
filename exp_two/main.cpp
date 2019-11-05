#include <algorithm>
#include <iostream>
#include <list>
#include <math.h>
#include <string>
#include <vector>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

const int InitPos = 90;

class Request {
public:
  string request_name;
  int request_track_no;
};

class Controller {
protected:
  vector<Request> requests;

public:
  Controller() {}
  Controller(Controller &from) { requests = from.requests; }
  Controller(Controller &&from) { requests = std::move(from.requests); }
  void receive_request(Request &req) { requests.push_back(req); }
  void receive_request(string request_name, int request_track_no) {
    Request req;
    req.request_name = request_name;
    req.request_track_no = request_track_no;
    requests.push_back(req);
  }
  void virtual schedule() = 0;
  void copy(Controller *from) { this->requests = from->requests; }
};

class FCFS_Controller : public Controller {
public:
  void schedule() override {
    int current_pos = InitPos;
    int step = 0;
    cout << "FCFS Order:" << endl;
    for (auto req : requests) {
      step += abs(current_pos - req.request_track_no);
      cout << req.request_name << "(" << req.request_track_no << ")"
           << " ";
      current_pos = req.request_track_no;
    }
    cout << endl;
    cout << "average step: "
         << static_cast<float>(step) / static_cast<float>(requests.size())
         << endl;
  }
};

class SSTF_Controller : public Controller {
public:
  void schedule() override {
    int current_pos = InitPos;
    int step = 0;
    int list_length = requests.size();
    cout << "SSTF Order:" << endl;
    std::vector<Request>::iterator shortest = requests.begin();
    while (!requests.empty()) {
      shortest = std::min_element(
          requests.begin(), requests.end(),
          [current_pos](const Request &req1, const Request &req2) {
            return abs(req1.request_track_no - current_pos) <
                   abs(req2.request_track_no - current_pos);
          });
      cout << shortest->request_name << "(" << shortest->request_track_no << ")"
           << " ";
      step += abs(shortest->request_track_no - current_pos);
      current_pos = shortest->request_track_no;
      requests.erase(shortest);
    }
    cout << endl;
    cout << "average step: "
         << static_cast<float>(step) / static_cast<float>(list_length) << endl;
  }
};

class SCAN_Controller : public Controller {
public:
  void schedule() override {
    int current_pos = InitPos;
    int step = 0;
    cout << "SCAN Order:" << endl;
    std::sort(requests.begin(), requests.end(),
              [](const Request &req1, const Request &req2) {
                return req1.request_track_no < req2.request_track_no;
              });
    std::vector<Request>::iterator pivot = requests.end();
    for (std::vector<Request>::iterator iter = requests.begin();
         iter != requests.end(); iter++) {
      if (iter->request_track_no < current_pos)
        continue;
      pivot = iter;
      break;
    }
    for (std::vector<Request>::iterator iter = pivot; iter != requests.end();
         iter++) {
      cout << iter->request_name << "(" << iter->request_track_no << ")"
           << " ";
      step += abs(iter->request_track_no - current_pos);
      current_pos = iter->request_track_no;
    }
    for (std::vector<Request>::reverse_iterator iter(pivot);
         iter != requests.rend(); iter++) {
      cout << iter->request_name << "(" << iter->request_track_no << ")"
           << " ";
      step += abs(iter->request_track_no - current_pos);
      current_pos = iter->request_track_no;
    }
    cout << endl;
    cout << "average step: "
         << static_cast<float>(step) / static_cast<float>(requests.size())
         << endl;
  }
};
int main(void) {
  FCFS_Controller c1;
  c1.receive_request("A", 30);
  c1.receive_request("B", 50);
  c1.receive_request("C", 100);
  c1.receive_request("D", 180);
  c1.receive_request("E", 20);
  c1.receive_request("F", 90);
  c1.receive_request("G", 150);
  c1.receive_request("H", 70);
  c1.receive_request("I", 80);
  c1.receive_request("J", 10);
  c1.receive_request("K", 160);
  c1.receive_request("L", 120);
  c1.receive_request("M", 40);
  c1.receive_request("N", 110);
  SSTF_Controller c2;
  c2.copy(&c1);
  SCAN_Controller c3;
  c3.copy(&c1);
  c1.schedule();
  c2.schedule();
  c3.schedule();
  return 0;
}

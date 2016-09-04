#include <chrono>

#ifndef SHARED_H
#define SHARED_H

// unamed namespace to be included in each test.cc file, yes I know I'm lazy
namespace
{

class Timer {
  typedef std::chrono::steady_clock Clock;
 public:
  Timer() : start(Clock::now()) {
  }

  long ElapsedMs() const {
    std::chrono::time_point<Clock> end = Clock::now();
    std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    return diff.count();
  }

  double ElapsedSec() const {
    return ElapsedMs() / 1000.0;
  }

 private:
  std::chrono::time_point<Clock> start;
};

void TestFunc() {
  std::cout << "I'm a function in C++\n";
}

std::string BuildString(const std::string& str) {
  if( str.empty() ) {
    return "x";
  }
  else {
    return "a";
  }
}

struct Bar {
  int x;
  Bar(int x_) : x(x_) {}
  int AddThis(int y) const { return x + y; }
};

void Print(const std::string& str) {
  std::cout << "Printing from C++ now\n";
  std::cout << str << "\n";
}

}  // namespace

#define TEST_COUNT 10
#define PERF_COUNT 10000
#define PERF_TIMES 4

#endif  // SHARED_H

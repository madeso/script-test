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

}  // namespace

#endif  // SHARED_H

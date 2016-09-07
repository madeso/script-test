#include <chrono>

#ifndef SHARED_H
#define SHARED_H

#define TEST_COUNT 10
#define PERF_COUNT 10000
#define PERF_TIMES 4

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

template<typename Lang>
struct LanguageTest {
  /*
  virtual int RunRec(int i) = 0;
  virtual int RunLoop(int i) = 0;
  virtual int RunStr(int i) = 0;
  */

  void PrintResult(const std::string& test, int v, double total) {
    std::cout << test << ":  " << v << " ms " << total/PERF_TIMES << "\n";
  }

  void Run() {
    Lang* lang = reinterpret_cast<Lang*>(this);

    std::cout << "\nFibonacci tests\n";
    std::cout << "//    1 2 3 5 8 13 21 34 55 89\n";

    std::cout << "Rec: ";
    for(int i=1; i<=TEST_COUNT; ++i) {
      int v = lang->RunRec(i);
      std::cout << " " << v;
    }
    std::cout << "\n";

    std::cout << "Loop:";
    for(int i=1; i<=TEST_COUNT; ++i) {
      int v = lang->RunLoop(i);
      std::cout << " " << v;
    }
    std::cout << "\n";

    std::cout << "\nString test:\n";
    const int s = lang->RunStr(6);
    std::cout << "6: " << s << "\n";

    int v = 0;
    double total = 0;

    std::cout << "\nPerfomance tests\n";
    v = 0;
    total = 0;
    for(int tot=0; tot<PERF_TIMES; ++tot){
      Timer timer;
      for (int i = 0; i < PERF_COUNT; ++i) {
        int t = lang->RunRec( (i%10) + 1 );
        if (t > 0) ++v;
      }
      total += timer.ElapsedSec();
      std::cout << ".";
    }
    PrintResult("rec", v, total);

    v = 0;
    total = 0;
    for(int tot=0; tot<PERF_TIMES; ++tot){
      Timer timer;
      for (int i = 0; i < PERF_COUNT; ++i) {
        int t = lang->RunLoop( (i%10) + 1 );
        if (t > 0) ++v;
      }
      total += timer.ElapsedSec();
      std::cout << ".";
    }
    PrintResult("loop", v, total);

    v = 0;
    total = 0;
    for(int tot=0; tot<PERF_TIMES; ++tot){
      Timer timer;
      for (int i = 0; i < PERF_COUNT; ++i) {
        int t = lang->RunStr( (i%10) + 1 );
        if (t > 0) ++v;
      }
      total += timer.ElapsedSec();
      std::cout << ".";
    }
    PrintResult("str", v, total);
  }
};

}  // namespace

#endif  // SHARED_H

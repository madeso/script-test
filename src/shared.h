#pragma once

#include <chrono>
#include <string>

constexpr int TEST_COUNT = 10;
constexpr int PERF_COUNT = 10000;
constexpr int PERF_TIMES = 4;

struct Timer
{
    using Clock = std::chrono::steady_clock;

    Timer();

    long ElapsedMs() const;

    double ElapsedSec() const;

    std::chrono::time_point<Clock> start;
};

void TestFunc();

std::string BuildString(const std::string& str);

struct Bar
{
    int x;

    Bar(int xx);

    int AddThis(int y) const;
};

void Print(const std::string& str);

struct LanguageTest
{
    virtual int RunRec(int i) = 0;
    virtual int RunLoop(int i) = 0;
    virtual int RunStr(int i) = 0;

    void PrintResult(const std::string& test, int v, double total);

    void Run();
};

#include "shared.h"

#include <iostream>

Timer::Timer()
    : start(Clock::now())
{
}

long Timer::ElapsedMs() const
{
    std::chrono::time_point<Clock> end = Clock::now();
    std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    return diff.count();
}

double Timer::ElapsedSec() const
{
    return ElapsedMs() / 1000.0;
}

void TestFunc()
{
    std::cout << "I'm a function in C++\n";
}

std::string BuildString(const std::string& str)
{
    if (str.empty())
    {
        return "x";
    }
    else
    {
        return "a";
    }
}

Bar::Bar(int xx)
    : x(xx)
{
}

int Bar::AddThis(int y) const
{
    return x + y;
}

void Print(const std::string& str)
{
    std::cout << "Printing from C++ now\n";
    std::cout << str << "\n";
}

void LanguageTest::PrintResult(const std::string& test, int v, double total)
{
    std::cout << test << ":  " << v << " ms " << total / PERF_TIMES << "\n";
}

void LanguageTest::Run()
{
    std::cout << "\nFibonacci tests\n";
    std::cout << "//    1 2 3 5 8 13 21 34 55 89\n";

    std::cout << "Rec: ";
    for (int i = 1; i <= TEST_COUNT; ++i)
    {
        int v = RunRec(i);
        std::cout << " " << v;
    }
    std::cout << "\n";

    std::cout << "Loop:";
    for (int i = 1; i <= TEST_COUNT; ++i)
    {
        int v = RunLoop(i);
        std::cout << " " << v;
    }
    std::cout << "\n";

    std::cout << "\nString test:\n";
    const int s = RunStr(6);
    std::cout << "6: " << s << "\n";

    int v = 0;
    double total = 0;

    std::cout << "\nPerfomance tests\n";
    v = 0;
    total = 0;
    for (int tot = 0; tot < PERF_TIMES; ++tot)
    {
        Timer timer;
        for (int i = 0; i < PERF_COUNT; ++i)
        {
            int t = RunRec((i % 10) + 1);
            if (t > 0)
                ++v;
        }
        total += timer.ElapsedSec();
        std::cout << ".";
    }
    PrintResult("rec", v, total);

    v = 0;
    total = 0;
    for (int tot = 0; tot < PERF_TIMES; ++tot)
    {
        Timer timer;
        for (int i = 0; i < PERF_COUNT; ++i)
        {
            int t = RunLoop((i % 10) + 1);
            if (t > 0)
                ++v;
        }
        total += timer.ElapsedSec();
        std::cout << ".";
    }
    PrintResult("loop", v, total);

    v = 0;
    total = 0;
    for (int tot = 0; tot < PERF_TIMES; ++tot)
    {
        Timer timer;
        for (int i = 0; i < PERF_COUNT; ++i)
        {
            int t = RunStr((i % 10) + 1);
            if (t > 0)
                ++v;
        }
        total += timer.ElapsedSec();
        std::cout << ".";
    }
    PrintResult("str", v, total);
}

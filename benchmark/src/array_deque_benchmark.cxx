#include "array_deque.h"
#include <benchmark/benchmark.h>
#include <deque>
#include <random>

#define INTCMP(a, b) ((a) == (b))
LTT_ARRAYDEQUE_INIT(INT, int, INTCMP)

#define NUMBERS       100000
#define ITERATIONSNUM (INT_MAX / NUMBERS)

int* MODEArray = new int[NUMBERS];

static void SetArray()
{
    std::random_device                 rd;             // 使用随机设备种子
    std::default_random_engine         eng(rd());      // 随机数引擎
    std::uniform_int_distribution<int> distr(1, 4);    // 定义分布范围
    for (int i = 0; i < NUMBERS; ++i) MODEArray[i] = distr(eng);
}

// 测试ArrayDeque_G头部插入性能
static void ArrayDeque_G_AddFirst_Test(benchmark::State& state)
{
    ArrayDeque(INT) ArrayDeque_G;
    for (auto _ : state)
    {
        ArrayDeque_Init(INT, &ArrayDeque_G);
        for (int i = 0; i < state.range(0); ++i) ArrayDeque_AddFirst(INT, &ArrayDeque_G, i);
        ArrayDeque_Destroy(INT, &ArrayDeque_G);
    }
}

// 测试ArrayDeque_G尾部插入性能
static void ArrayDeque_G_AddLast_Test(benchmark::State& state)
{
    ArrayDeque(INT) ArrayDeque_G;
    for (auto _ : state)
    {
        ArrayDeque_Init(INT, &ArrayDeque_G);
        for (int i = 0; i < state.range(0); ++i) ArrayDeque_AddLast(INT, &ArrayDeque_G, i);
        ArrayDeque_Destroy(INT, &ArrayDeque_G);
    }
}

// 测试ArrayDeque_G头部删除性能
static void ArrayDeque_G_DeleteFirst_Test(benchmark::State& state)
{
    int Temp;
    ArrayDeque(INT) ArrayDeque_G;
    for (auto _ : state)
    {
        ArrayDeque_Init(INT, &ArrayDeque_G);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) ArrayDeque_AddLast(INT, &ArrayDeque_G, i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) ArrayDeque_DeleteFirst(INT, &ArrayDeque_G, &Temp);
        ArrayDeque_Destroy(INT, &ArrayDeque_G);
    }
}

// 测试ArrayDeque_G尾部删除性能
static void ArrayDeque_G_DeleteLast_Test(benchmark::State& state)
{
    int Temp;
    ArrayDeque(INT) ArrayDeque_G;
    for (auto _ : state)
    {
        ArrayDeque_Init(INT, &ArrayDeque_G);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) ArrayDeque_AddLast(INT, &ArrayDeque_G, i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) ArrayDeque_DeleteLast(INT, &ArrayDeque_G, &Temp);
        ArrayDeque_Destroy(INT, &ArrayDeque_G);
    }
}

// 测试ArrayDeque_G随机插入性能
static void ArrayDeque_G_random_Test(benchmark::State& state)
{
    int Temp;
    ArrayDeque(INT) ArrayDeque_G;
    for (auto _ : state)
    {
        ArrayDeque_Init(INT, &ArrayDeque_G);
        for (int i = 0; i < state.range(0); ++i)
        {
            switch (MODEArray[i])
            {
                case 1 : ArrayDeque_AddFirst(INT, &ArrayDeque_G, i); break;
                case 2 : ArrayDeque_AddLast(INT, &ArrayDeque_G, i); break;
                case 3 :
                    if (!ArrayDeque_IsEmpty(&ArrayDeque_G)) ArrayDeque_DeleteFirst(INT, &ArrayDeque_G, &Temp);
                    break;
                case 4 :
                    if (!ArrayDeque_IsEmpty(&ArrayDeque_G)) ArrayDeque_DeleteLast(INT, &ArrayDeque_G, &Temp);
                    break;
            }
        }
        ArrayDeque_Destroy(INT, &ArrayDeque_G);
    }
}

// 测试deque头部插入性能
static void deque_AddFirst_Test(benchmark::State& state)
{
    std::deque<int> deque;
    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i) deque.push_front(i);
        deque.clear();
    }
}

// 测试deque尾部插入性能
static void deque_AddLast_Test(benchmark::State& state)
{
    std::deque<int> deque;
    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i) deque.push_back(i);
        deque.clear();
    }
}

// 测试deque头部删除性能
static void deque_DeleteFirst_Test(benchmark::State& state)
{
    std::deque<int> deque;
    for (auto _ : state)
    {
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) deque.push_back(i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) deque.pop_front();
        deque.clear();
    }
}

// 测试deque尾部删除性能
static void deque_DeleteLast_Test(benchmark::State& state)
{
    std::deque<int> deque;
    for (auto _ : state)
    {
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) deque.push_back(i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) deque.pop_back();
        deque.clear();
    }
}

// 测试deque随机插入性能
static void deque_G_random_Test(benchmark::State& state)
{
    std::deque<int> deque;
    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i)
        {
            switch (MODEArray[i])
            {
                case 1 : deque.push_front(i); break;
                case 2 : deque.push_back(i); break;
                case 3 :
                    if (!deque.empty()) deque.pop_front();
                    break;
                case 4 :
                    if (!deque.empty()) deque.pop_back();
                    break;
            }
        }
        deque.clear();
    }
}

BENCHMARK(ArrayDeque_G_AddFirst_Test)->Arg(NUMBERS);
BENCHMARK(deque_AddFirst_Test)->Arg(NUMBERS);
BENCHMARK(ArrayDeque_G_AddLast_Test)->Arg(NUMBERS);
BENCHMARK(deque_AddLast_Test)->Arg(NUMBERS);
BENCHMARK(ArrayDeque_G_DeleteFirst_Test)->Arg(NUMBERS)->Iterations(ITERATIONSNUM);
BENCHMARK(deque_DeleteFirst_Test)->Arg(NUMBERS);
BENCHMARK(ArrayDeque_G_DeleteLast_Test)->Arg(NUMBERS)->Iterations(ITERATIONSNUM);
BENCHMARK(deque_DeleteLast_Test)->Arg(NUMBERS);
BENCHMARK(ArrayDeque_G_random_Test)->Arg(NUMBERS);
BENCHMARK(deque_G_random_Test)->Arg(NUMBERS);

int main(int argc, char** argv)
{
    SetArray();    //  设置模式数组
    char  arg0_default[] = "benchmark";
    char* args_default   = arg0_default;
    if (!argv)
    {
        argc = 1;
        argv = &args_default;
    }
    ::benchmark ::Initialize(&argc, argv);
    if (::benchmark ::ReportUnrecognizedArguments(argc, argv)) return 1;
    ::benchmark ::RunSpecifiedBenchmarks();
    ::benchmark ::Shutdown();
    return 0;
}

int main(int, char**);
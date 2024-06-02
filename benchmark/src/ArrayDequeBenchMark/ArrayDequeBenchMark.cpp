#include <benchmark/benchmark.h>
#include <deque>
#include "ArrayDeque_G.h"

#define INTCMP(a, b) ((a) == (b))
LTT_ARRAYDEQUE_INIT(INT, int, INTCMP)

#define NUMBERS       100000
#define ITERATIONSNUM (INT_MAX / NUMBERS)

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

BENCHMARK(ArrayDeque_G_AddFirst_Test)->Arg(NUMBERS);
BENCHMARK(deque_AddFirst_Test)->Arg(NUMBERS);
BENCHMARK(ArrayDeque_G_AddLast_Test)->Arg(NUMBERS);
BENCHMARK(deque_AddLast_Test)->Arg(NUMBERS);
BENCHMARK(ArrayDeque_G_DeleteFirst_Test)->Arg(NUMBERS)->Iterations(ITERATIONSNUM);
BENCHMARK(deque_DeleteFirst_Test)->Arg(NUMBERS);
BENCHMARK(ArrayDeque_G_DeleteLast_Test)->Arg(NUMBERS)->Iterations(ITERATIONSNUM);
BENCHMARK(deque_DeleteLast_Test)->Arg(NUMBERS);
BENCHMARK_MAIN();
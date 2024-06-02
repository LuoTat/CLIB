#include <benchmark/benchmark.h>
#include <deque>
#include "ArrayDeque_G.h"

#define INTCMP(a, b) ((a) == (b))
LTT_ARRAYDEQUE_INIT(INT, int, INTCMP)

#define START 10000
#define LIMIT 100000000

// 测试ArrayDeque_G头部插入性能
static void ArrayDeque_G_AddFirst_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        ArrayDeque(INT) ArrayDeque_G;
        ArrayDeque_Init(INT, &ArrayDeque_G);
        for (int i = 0; i < state.range(0); ++i) { ArrayDeque_AddFirst(INT, &ArrayDeque_G, i); }
        ArrayDeque_Destroy(INT, &ArrayDeque_G);
    }
    state.SetComplexityN(state.range(0));
}

// 测试ArrayDeque_G尾部插入性能
static void ArrayDeque_G_AddLast_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        ArrayDeque(INT) ArrayDeque_G;
        ArrayDeque_Init(INT, &ArrayDeque_G);
        for (int i = 0; i < state.range(0); ++i) { ArrayDeque_AddLast(INT, &ArrayDeque_G, i); }
        ArrayDeque_Destroy(INT, &ArrayDeque_G);
    }
    state.SetComplexityN(state.range(0));
}

// 测试ArrayDeque_G头部删除性能
static void ArrayDeque_G_DeleteFirst_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        ArrayDeque(INT) ArrayDeque_G;
        ArrayDeque_Init(INT, &ArrayDeque_G);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) { ArrayDeque_AddLast(INT, &ArrayDeque_G, i); }
        state.ResumeTiming();
        int Temp;
        for (int i = 0; i < state.range(0); ++i) { ArrayDeque_DeleteFirst(INT, &ArrayDeque_G, &Temp); }
        ArrayDeque_Destroy(INT, &ArrayDeque_G);
    }
    state.SetComplexityN(state.range(0));
}

// 测试ArrayDeque_G尾部删除性能
static void ArrayDeque_G_DeleteLast_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        ArrayDeque(INT) ArrayDeque_G;
        ArrayDeque_Init(INT, &ArrayDeque_G);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) { ArrayDeque_AddLast(INT, &ArrayDeque_G, i); }
        state.ResumeTiming();
        int Temp;
        for (int i = 0; i < state.range(0); ++i) { ArrayDeque_DeleteLast(INT, &ArrayDeque_G, &Temp); }
        ArrayDeque_Destroy(INT, &ArrayDeque_G);
    }
    state.SetComplexityN(state.range(0));
}

// 测试deque头部插入性能
static void deque_AddFirst_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::deque<int> deque;
        for (int i = 0; i < state.range(0); ++i) { deque.push_front(i); }
    }
    state.SetComplexityN(state.range(0));
}

// 测试deque尾部插入性能
static void deque_AddLast_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::deque<int> deque;
        for (int i = 0; i < state.range(0); ++i) { deque.push_back(i); }
    }
    state.SetComplexityN(state.range(0));
}

// 测试deque头部删除性能
static void deque_DeleteFirst_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::deque<int> deque;
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) { deque.push_back(i); }
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) { deque.pop_front(); }
    }
    state.SetComplexityN(state.range(0));
}

// 测试deque尾部删除性能
static void deque_DeleteLast_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::deque<int> deque;
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) { deque.push_back(i); }
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) { deque.pop_back(); }
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(ArrayDeque_G_AddFirst_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
// BENCHMARK(deque_AddFirst_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
BENCHMARK(ArrayDeque_G_AddLast_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
// BENCHMARK(deque_AddLast_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
BENCHMARK(ArrayDeque_G_DeleteFirst_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
// BENCHMARK(deque_DeleteFirst_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
BENCHMARK(ArrayDeque_G_DeleteLast_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
// BENCHMARK(deque_DeleteLast_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
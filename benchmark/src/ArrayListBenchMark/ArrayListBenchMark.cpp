#include <benchmark/benchmark.h>
#include <vector>
#include "ArrayList_G.h"

#define INTCMP(a, b) ((a) == (b))
LTT_ARRAYLIST_INIT(INT, int, INTCMP)

#define START 10000
#define LIMIT 100000000

// 测试ArrayList_G头部插入性能
static void ArrayList_G_AddFirst_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        ArrayList(INT) ArrayList_G;
        ArrayList_Init(INT, &ArrayList_G);
        for (int i = 0; i < state.range(0); ++i) { ArrayList_AddFirst(INT, &ArrayList_G, i); }
        ArrayList_Destroy(INT, &ArrayList_G);
    }
    state.SetComplexityN(state.range(0));
}

// 测试ArrayList_G尾部插入性能
static void ArrayList_G_AddLast_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        ArrayList(INT) ArrayList_G;
        ArrayList_Init(INT, &ArrayList_G);
        for (int i = 0; i < state.range(0); ++i) { ArrayList_AddLast(INT, &ArrayList_G, i); }
        ArrayList_Destroy(INT, &ArrayList_G);
    }
    state.SetComplexityN(state.range(0));
}

// 测试ArrayList_G头部删除性能
static void ArrayList_G_DeleteFirst_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        ArrayList(INT) ArrayList_G;
        ArrayList_Init(INT, &ArrayList_G);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) { ArrayList_AddLast(INT, &ArrayList_G, i); }
        state.ResumeTiming();
        int Temp;
        for (int i = 0; i < state.range(0); ++i) { ArrayList_DeleteFirst(INT, &ArrayList_G, &Temp); }
        ArrayList_Destroy(INT, &ArrayList_G);
    }
    state.SetComplexityN(state.range(0));
}

// 测试ArrayList_G尾部删除性能
static void ArrayList_G_DeleteLast_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        ArrayList(INT) ArrayList_G;
        ArrayList_Init(INT, &ArrayList_G);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) { ArrayList_AddLast(INT, &ArrayList_G, i); }
        state.ResumeTiming();
        int Temp;
        for (int i = 0; i < state.range(0); ++i) { ArrayList_DeleteLast(INT, &ArrayList_G, &Temp); }
        ArrayList_Destroy(INT, &ArrayList_G);
    }
    state.SetComplexityN(state.range(0));
}

// 测试vector头部插入性能
static void vector_AddFirst_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::vector<int> vector;
        for (int i = 0; i < state.range(0); ++i) { vector.insert(vector.begin(), i); }
    }
    state.SetComplexityN(state.range(0));
}

// 测试vector尾部插入性能
static void vector_AddLast_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::vector<int> vector;
        for (int i = 0; i < state.range(0); ++i) { vector.push_back(i); }
    }
    state.SetComplexityN(state.range(0));
}

// 测试vector头部删除性能
static void vector_DeleteFirst_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::vector<int> vector;
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) { vector.push_back(i); }
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) { vector.erase(vector.begin()); }
    }
    state.SetComplexityN(state.range(0));
}

// 测试vector尾部删除性能
static void vector_DeleteLast_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::vector<int> vector;
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) { vector.push_back(i); }
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) { vector.pop_back(); }
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(ArrayList_G_AddFirst_Test)->RangeMultiplier(10)->Range(10, 100000)->Complexity()->Unit(benchmark::kMillisecond);
BENCHMARK(vector_AddFirst_Test)->RangeMultiplier(10)->Range(10, 100000)->Complexity()->Unit(benchmark::kMillisecond);
BENCHMARK(ArrayList_G_AddLast_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
BENCHMARK(vector_AddLast_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
BENCHMARK(ArrayList_G_DeleteFirst_Test)->RangeMultiplier(10)->Range(10, 100000)->Complexity()->Unit(benchmark::kMillisecond);
BENCHMARK(vector_DeleteFirst_Test)->RangeMultiplier(10)->Range(10, 100000)->Complexity()->Unit(benchmark::kMillisecond);
BENCHMARK(ArrayList_G_DeleteLast_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
BENCHMARK(vector_DeleteLast_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
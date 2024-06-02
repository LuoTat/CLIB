#include <benchmark/benchmark.h>
#include <vector>
#include "ArrayList_G.h"

#define INTCMP(a, b) ((a) == (b))
LTT_ARRAYLIST_INIT(INT, int, INTCMP)

#define NUMBERS       100000
#define ITERATIONSNUM (INT_MAX / NUMBERS)

// 测试ArrayList_G头部插入性能
static void ArrayList_G_AddFirst_Test(benchmark::State& state)
{
    ArrayList(INT) ArrayList_G;
    for (auto _ : state)
    {
        ArrayList_Init(INT, &ArrayList_G);
        for (int i = 0; i < state.range(0); ++i) ArrayList_AddFirst(INT, &ArrayList_G, i);
        ArrayList_Destroy(INT, &ArrayList_G);
    }
}

// 测试ArrayList_G尾部插入性能
static void ArrayList_G_AddLast_Test(benchmark::State& state)
{
    ArrayList(INT) ArrayList_G;
    for (auto _ : state)
    {
        ArrayList_Init(INT, &ArrayList_G);
        for (int i = 0; i < state.range(0); ++i) ArrayList_AddLast(INT, &ArrayList_G, i);
        ArrayList_Destroy(INT, &ArrayList_G);
    }
}

// 测试ArrayList_G头部删除性能
static void ArrayList_G_DeleteFirst_Test(benchmark::State& state)
{
    int Temp;
    ArrayList(INT) ArrayList_G;
    for (auto _ : state)
    {
        ArrayList_Init(INT, &ArrayList_G);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) ArrayList_AddLast(INT, &ArrayList_G, i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) ArrayList_DeleteFirst(INT, &ArrayList_G, &Temp);
        ArrayList_Destroy(INT, &ArrayList_G);
    }
}

// 测试ArrayList_G尾部删除性能
static void ArrayList_G_DeleteLast_Test(benchmark::State& state)
{
    int Temp;
    ArrayList(INT) ArrayList_G;
    for (auto _ : state)
    {
        ArrayList_Init(INT, &ArrayList_G);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) ArrayList_AddLast(INT, &ArrayList_G, i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) ArrayList_DeleteLast(INT, &ArrayList_G, &Temp);
        ArrayList_Destroy(INT, &ArrayList_G);
    }
}

// 测试vector头部插入性能
static void vector_AddFirst_Test(benchmark::State& state)
{
    std::vector<int> vector;
    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i) vector.insert(vector.begin(), i);
        vector.clear();
    }
}

// 测试vector尾部插入性能
static void vector_AddLast_Test(benchmark::State& state)
{
    std::vector<int> vector;
    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i) vector.push_back(i);
        vector.clear();
    }
}

// 测试vector头部删除性能
static void vector_DeleteFirst_Test(benchmark::State& state)
{
    std::vector<int> vector;
    for (auto _ : state)
    {
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) vector.push_back(i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) vector.erase(vector.begin());
        vector.clear();
    }
}

// 测试vector尾部删除性能
static void vector_DeleteLast_Test(benchmark::State& state)
{
    std::vector<int> vector;
    for (auto _ : state)
    {
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) vector.push_back(i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) vector.pop_back();
        vector.clear();
    }
}

BENCHMARK(ArrayList_G_AddFirst_Test)->Arg(NUMBERS);
BENCHMARK(vector_AddFirst_Test)->Arg(NUMBERS);
BENCHMARK(ArrayList_G_AddLast_Test)->Arg(NUMBERS);
BENCHMARK(vector_AddLast_Test)->Arg(NUMBERS);
BENCHMARK(ArrayList_G_DeleteFirst_Test)->Arg(NUMBERS);
BENCHMARK(vector_DeleteFirst_Test)->Arg(NUMBERS);
BENCHMARK(ArrayList_G_DeleteLast_Test)->Arg(NUMBERS)->Iterations(ITERATIONSNUM);
BENCHMARK(vector_DeleteLast_Test)->Arg(NUMBERS)->Iterations(ITERATIONSNUM);
BENCHMARK_MAIN();
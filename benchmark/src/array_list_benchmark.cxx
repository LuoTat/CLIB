#include "array_list.h"
#include <benchmark/benchmark.h>
#include <random>
#include <vector>

#define INTCMP(a, b) ((a) == (b))
LTT_ARRAYLIST_INIT(INT, int, INTCMP)

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

// 测试ArrayList_G随机插入性能
static void ArrayList_G_random_Test(benchmark::State& state)
{
    int Temp;
    ArrayList(INT) ArrayList_G;
    for (auto _ : state)
    {
        ArrayList_Init(INT, &ArrayList_G);
        for (int i = 0; i < state.range(0); ++i)
        {
            switch (MODEArray[i])
            {
                case 1 : ArrayList_AddFirst(INT, &ArrayList_G, i); break;
                case 2 : ArrayList_AddLast(INT, &ArrayList_G, i); break;
                case 3 :
                    if (!ArrayList_IsEmpty(&ArrayList_G)) ArrayList_DeleteFirst(INT, &ArrayList_G, &Temp);
                    break;
                case 4 :
                    if (!ArrayList_IsEmpty(&ArrayList_G)) ArrayList_DeleteLast(INT, &ArrayList_G, &Temp);
                    break;
            }
        }
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

// 测试vector随机插入性能
static void vector_random_Test(benchmark::State& state)
{
    std::vector<int> vector;
    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i)
        {
            switch (MODEArray[i])
            {
                case 1 : vector.insert(vector.begin(), i); break;
                case 2 : vector.push_back(i); break;
                case 3 :
                    if (!vector.empty()) vector.erase(vector.begin());
                    break;
                case 4 :
                    if (!vector.empty()) vector.pop_back();
                    break;
            }
        }
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
BENCHMARK(ArrayList_G_random_Test)->Arg(NUMBERS);
BENCHMARK(vector_random_Test)->Arg(NUMBERS);

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
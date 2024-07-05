#include <benchmark/benchmark.h>
#include <limits.h>
#include <list>
#include <random>
#include "LinkedList_G.h"

#define INTCMP(a, b) ((a) == (b))
LTT_LINKEDLIST_INIT(INT, int, INTCMP)

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

// 测试LinkedList_G头部插入性能
static void LinkedList_G_AddFirst_Test(benchmark::State& state)
{
    LinkedList(INT) LinkedList_G;
    for (auto _ : state)
    {
        LinkedList_Init(INT, &LinkedList_G);
        for (int i = 0; i < state.range(0); ++i) LinkedList_AddFirst(INT, &LinkedList_G, i);
        LinkedList_Destroy(INT, &LinkedList_G);
    }
}

// 测试LinkedList_G尾部插入性能
static void LinkedList_G_AddLast_Test(benchmark::State& state)
{
    LinkedList(INT) LinkedList_G;
    for (auto _ : state)
    {
        LinkedList_Init(INT, &LinkedList_G);
        for (int i = 0; i < state.range(0); ++i) LinkedList_AddLast(INT, &LinkedList_G, i);
        LinkedList_Destroy(INT, &LinkedList_G);
    }
}

// 测试LinkedList_G头部删除性能
static void LinkedList_G_DeleteFirst_Test(benchmark::State& state)
{
    int Temp;
    LinkedList(INT) LinkedList_G;
    for (auto _ : state)
    {
        LinkedList_Init(INT, &LinkedList_G);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) LinkedList_AddLast(INT, &LinkedList_G, i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) LinkedList_DeleteFirst(INT, &LinkedList_G, &Temp);
        LinkedList_Destroy(INT, &LinkedList_G);
    }
}

// 测试LinkedList_G尾部删除性能
static void LinkedList_G_DeleteLast_Test(benchmark::State& state)
{
    int Temp;
    LinkedList(INT) LinkedList_G;
    for (auto _ : state)
    {
        LinkedList_Init(INT, &LinkedList_G);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) LinkedList_AddLast(INT, &LinkedList_G, i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) LinkedList_DeleteLast(INT, &LinkedList_G, &Temp);
        LinkedList_Destroy(INT, &LinkedList_G);
    }
}

// 测试LinkedList_G随机插入性能
static void LinkedList_G_random_Test(benchmark::State& state)
{
    int Temp;
    LinkedList(INT) LinkedList_G;
    for (auto _ : state)
    {
        LinkedList_Init(INT, &LinkedList_G);
        for (int i = 0; i < state.range(0); ++i)
        {
            switch (MODEArray[i])
            {
                case 1 : LinkedList_AddFirst(INT, &LinkedList_G, i); break;
                case 2 : LinkedList_AddLast(INT, &LinkedList_G, i); break;
                case 3 :
                    if (!LinkedList_IsEmpty(&LinkedList_G)) LinkedList_DeleteFirst(INT, &LinkedList_G, &Temp);
                    break;
                case 4 :
                    if (!LinkedList_IsEmpty(&LinkedList_G)) LinkedList_DeleteLast(INT, &LinkedList_G, &Temp);
                    break;
            }
        }
        LinkedList_Destroy(INT, &LinkedList_G);
    }
}

// 测试list头部插入性能
static void list_AddFirst_Test(benchmark::State& state)
{
    std::list<int> list;
    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i) list.push_front(i);
        list.clear();
    }
}

// 测试list尾部插入性能
static void list_AddLast_Test(benchmark::State& state)
{
    std::list<int> list;
    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i) list.push_back(i);
        list.clear();
    }
}

// 测试list头部删除性能
static void list_DeleteFirst_Test(benchmark::State& state)
{
    std::list<int> list;
    for (auto _ : state)
    {
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) list.push_back(i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) list.pop_front();
        list.clear();
    }
}

// 测试list尾部删除性能
static void list_DeleteLast_Test(benchmark::State& state)
{
    std::list<int> list;
    for (auto _ : state)
    {
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) list.push_back(i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) list.pop_back();
        list.clear();
    }
}

// 测试list随机插入性能
static void list_random_Test(benchmark::State& state)
{
    std::list<int> list;
    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i)
        {
            switch (MODEArray[i])
            {
                case 1 : list.push_front(i); break;
                case 2 : list.push_back(i); break;
                case 3 :
                    if (!list.empty()) list.pop_front();
                    break;
                case 4 :
                    if (!list.empty()) list.pop_back();
                    break;
            }
        }
        list.clear();
    }
}

BENCHMARK(LinkedList_G_AddFirst_Test)->Arg(NUMBERS);
BENCHMARK(list_AddFirst_Test)->Arg(NUMBERS);
BENCHMARK(LinkedList_G_AddLast_Test)->Arg(NUMBERS);
BENCHMARK(list_AddLast_Test)->Arg(NUMBERS);
BENCHMARK(LinkedList_G_DeleteFirst_Test)->Arg(NUMBERS);
BENCHMARK(list_DeleteFirst_Test)->Arg(NUMBERS);
BENCHMARK(LinkedList_G_DeleteLast_Test)->Arg(NUMBERS);
BENCHMARK(list_DeleteLast_Test)->Arg(NUMBERS);
BENCHMARK(LinkedList_G_random_Test)->Arg(NUMBERS);
BENCHMARK(list_random_Test)->Arg(NUMBERS);

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
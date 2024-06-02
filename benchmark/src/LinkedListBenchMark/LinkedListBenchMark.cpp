#include <benchmark/benchmark.h>
#include <list>
#include <limits.h>
#include "LinkedList_G.h"

#define INTCMP(a, b) ((a) == (b))
LTT_LINKEDLIST_INIT(INT, int, INTCMP)

#define NUMBERS       100000
#define ITERATIONSNUM (INT_MAX / NUMBERS)

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

BENCHMARK(LinkedList_G_AddFirst_Test)->Arg(NUMBERS);
BENCHMARK(list_AddFirst_Test)->Arg(NUMBERS);
BENCHMARK(LinkedList_G_AddLast_Test)->Arg(NUMBERS);
BENCHMARK(list_AddLast_Test)->Arg(NUMBERS);
BENCHMARK(LinkedList_G_DeleteFirst_Test)->Arg(NUMBERS);
BENCHMARK(list_DeleteFirst_Test)->Arg(NUMBERS);
BENCHMARK(LinkedList_G_DeleteLast_Test)->Arg(NUMBERS);
BENCHMARK(list_DeleteLast_Test)->Arg(NUMBERS);
BENCHMARK_MAIN();

//#include <benchmark/benchmark.h>
//#include <list>
//#include "LinkedList_G.h"
//
//#define INTCMP(a, b) ((a) == (b))
//LTT_LINKEDLIST_INIT(INT, int, INTCMP)
//
//#define START 10000
//#define LIMIT 100000000
//
//// 测试LinkedList_G头部插入性能
//static void LinkedList_G_AddFirst_Test(benchmark::State& state)
//{
//    LinkedList(INT) LinkedList_G;
//    for (auto _ : state)
//    {
//        LinkedList_Init(INT, &LinkedList_G);
//        for (int i = 0; i < state.range(0); ++i) { LinkedList_AddFirst(INT, &LinkedList_G, i); }
//        LinkedList_Destroy(INT, &LinkedList_G);
//    }
//    state.SetComplexityN(state.range(0));
//}
//
//// 测试LinkedList_G尾部插入性能
//static void LinkedList_G_AddLast_Test(benchmark::State& state)
//{
//    LinkedList(INT) LinkedList_G;
//    for (auto _ : state)
//    {
//        LinkedList_Init(INT, &LinkedList_G);
//        for (int i = 0; i < state.range(0); ++i) { LinkedList_AddLast(INT, &LinkedList_G, i); }
//        LinkedList_Destroy(INT, &LinkedList_G);
//    }
//    state.SetComplexityN(state.range(0));
//}
//
//// 测试LinkedList_G头部删除性能
//static void LinkedList_G_DeleteFirst_Test(benchmark::State& state)
//{
//    LinkedList(INT) LinkedList_G;
//    for (auto _ : state)
//    {
//        LinkedList_Init(INT, &LinkedList_G);
//        state.PauseTiming();
//        for (int i = 0; i < state.range(0); ++i) { LinkedList_AddLast(INT, &LinkedList_G, i); }
//        state.ResumeTiming();
//        int Temp;
//        for (int i = 0; i < state.range(0); ++i) { LinkedList_DeleteFirst(INT, &LinkedList_G, &Temp); }
//        LinkedList_Destroy(INT, &LinkedList_G);
//    }
//    state.SetComplexityN(state.range(0));
//}
//
//// 测试LinkedList_G尾部删除性能
//static void LinkedList_G_DeleteLast_Test(benchmark::State& state)
//{
//    LinkedList(INT) LinkedList_G;
//    for (auto _ : state)
//    {
//        LinkedList_Init(INT, &LinkedList_G);
//        state.PauseTiming();
//        for (int i = 0; i < state.range(0); ++i) { LinkedList_AddLast(INT, &LinkedList_G, i); }
//        state.ResumeTiming();
//        int Temp;
//        for (int i = 0; i < state.range(0); ++i) { LinkedList_DeleteLast(INT, &LinkedList_G, &Temp); }
//        LinkedList_Destroy(INT, &LinkedList_G);
//    }
//    state.SetComplexityN(state.range(0));
//}
//
//// 测试list头部插入性能
//static void list_AddFirst_Test(benchmark::State& state)
//{
//    for (auto _ : state)
//    {
//        std::list<int> list;
//        for (int i = 0; i < state.range(0); ++i) { list.push_front(i); }
//        list.clear();
//    }
//    state.SetComplexityN(state.range(0));
//}
//
//// 测试list尾部插入性能
//static void list_AddLast_Test(benchmark::State& state)
//{
//    for (auto _ : state)
//    {
//        std::list<int> list;
//        for (int i = 0; i < state.range(0); ++i) { list.push_back(i); }
//        list.clear();
//    }
//    state.SetComplexityN(state.range(0));
//}
//
//// 测试list头部删除性能
//static void list_DeleteFirst_Test(benchmark::State& state)
//{
//    for (auto _ : state)
//    {
//        std::list<int> list;
//        state.PauseTiming();
//        for (int i = 0; i < state.range(0); ++i) { list.push_back(i); }
//        state.ResumeTiming();
//        for (int i = 0; i < state.range(0); ++i) { list.pop_front(); }
//        list.clear();
//    }
//    state.SetComplexityN(state.range(0));
//}
//
//// 测试list尾部删除性能
//static void list_DeleteLast_Test(benchmark::State& state)
//{
//    for (auto _ : state)
//    {
//        std::list<int> list;
//        state.PauseTiming();
//        for (int i = 0; i < state.range(0); ++i) { list.push_back(i); }
//        state.ResumeTiming();
//        for (int i = 0; i < state.range(0); ++i) { list.pop_back(); }
//        list.clear();
//    }
//    state.SetComplexityN(state.range(0));
//}
//
//BENCHMARK(LinkedList_G_AddFirst_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
//BENCHMARK(list_AddFirst_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
//BENCHMARK(LinkedList_G_AddLast_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
//BENCHMARK(list_AddLast_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
//BENCHMARK(LinkedList_G_DeleteFirst_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
//BENCHMARK(list_DeleteFirst_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
//BENCHMARK(LinkedList_G_DeleteLast_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
//BENCHMARK(list_DeleteLast_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
//
//BENCHMARK_MAIN();

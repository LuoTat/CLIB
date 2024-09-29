#include "ArrayStack_G.h"
#include <benchmark/benchmark.h>
#include <random>
#include <stack>

#define INTCMP(a, b) ((a) == (b))
LTT_ARRAYSTACK_INIT(INT, int, INTCMP, LTT_alloc);

#define NUMBERS       100000
#define ITERATIONSNUM (INT_MAX / NUMBERS)

int* MODEArray = new int[NUMBERS];

static void SetArray()
{
    std::random_device                 rd;             // 使用随机设备种子
    std::default_random_engine         eng(rd());      // 随机数引擎
    std::uniform_int_distribution<int> distr(1, 2);    // 定义分布范围
    for (int i = 0; i < NUMBERS; ++i) MODEArray[i] = distr(eng);
}

// 测试ArrayStack_G Push性能
static void ArrayStack_G_Push_Test(benchmark::State& state)
{
    ArrayStack(INT) ArrayStack_G;
    for (auto _ : state)
    {
        ArrayStack_Init(INT, &ArrayStack_G);
        for (int i = 0; i < state.range(0); ++i) ArrayStack_Push(INT, &ArrayStack_G, i);
        ArrayStack_Destroy(INT, &ArrayStack_G);
    }
}

// 测试ArrayStack_G Pop性能
static void ArrayStack_G_Pop_Test(benchmark::State& state)
{
    int Temp;
    ArrayStack(INT) ArrayStack_G;
    for (auto _ : state)
    {
        ArrayStack_Init(INT, &ArrayStack_G);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) ArrayStack_Push(INT, &ArrayStack_G, i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) ArrayStack_Pop(INT, &ArrayStack_G, &Temp);
        ArrayStack_Destroy(INT, &ArrayStack_G);
    }
}

// 测试ArrayStack_G随机插入性能
static void ArrayStack_G_random_Test(benchmark::State& state)
{
    int Temp;
    ArrayStack(INT) ArrayStack_G;
    for (auto _ : state)
    {
        ArrayStack_Init(INT, &ArrayStack_G);
        for (int i = 0; i < state.range(0); ++i)
        {
            switch (MODEArray[i])
            {
                case 1 : ArrayStack_Push(INT, &ArrayStack_G, i); break;
                case 2 :
                    if (!ArrayStack_IsEmpty(&ArrayStack_G)) ArrayStack_Pop(INT, &ArrayStack_G, &Temp);
                    break;
            }
        }
        ArrayStack_Destroy(INT, &ArrayStack_G);
    }
}

// 测试stack Push性能
static void stack_Push_Test(benchmark::State& state)
{
    std::stack<int> stack;
    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i) stack.push(i);
    }
}

// 测试stack Pop性能
static void stack_Pop_Test(benchmark::State& state)
{
    std::stack<int> stack;
    for (auto _ : state)
    {
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) stack.push(i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) stack.pop();
    }
}

// 测试stack 随机插入性能
static void stack_random_Test(benchmark::State& state)
{
    std::stack<int> stack;
    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i)
        {
            switch (MODEArray[i])
            {
                case 1 : stack.push(i); break;
                case 2 :
                    if (!stack.empty()) stack.pop();
                    break;
            }
        }
    }
}

BENCHMARK(ArrayStack_G_Push_Test)->Arg(NUMBERS);
BENCHMARK(stack_Push_Test)->Arg(NUMBERS);
BENCHMARK(ArrayStack_G_Pop_Test)->Arg(NUMBERS)->Iterations(ITERATIONSNUM);
BENCHMARK(stack_Pop_Test)->Arg(NUMBERS)->Iterations(ITERATIONSNUM);
BENCHMARK(ArrayStack_G_random_Test)->Arg(NUMBERS);
BENCHMARK(stack_random_Test)->Arg(NUMBERS);

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
#include <benchmark/benchmark.h>
#include <stack>
#include "ArrayStack_G.h"

#define INTCMP(a, b) ((a) == (b))
LTT_ARRAYSTACK_INIT(INT, int, INTCMP)

#define NUMBERS       100000
#define ITERATIONSNUM (INT_MAX / NUMBERS)

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

BENCHMARK(ArrayStack_G_Push_Test)->Arg(NUMBERS);
BENCHMARK(stack_Push_Test)->Arg(NUMBERS);
BENCHMARK(ArrayStack_G_Pop_Test)->Arg(NUMBERS)->Iterations(ITERATIONSNUM);
BENCHMARK(stack_Pop_Test)->Arg(NUMBERS)->Iterations(ITERATIONSNUM);
BENCHMARK_MAIN();
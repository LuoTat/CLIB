#include <benchmark/benchmark.h>
#include <stack>
#include "ArrayStack_G.h"

#define INTCMP(a, b) ((a) == (b))
LTT_ARRAYSTACK_INIT(INT, int, INTCMP)

#define START 10000
#define LIMIT 100000000

// 测试ArrayStack_G入栈性能
static void ArrayStack_G_Push_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        ArrayStack(INT) ArrayStack_G;
        ArrayStack_Init(INT, &ArrayStack_G);
        for (int i = 0; i < state.range(0); ++i) { ArrayStack_Push(INT, &ArrayStack_G, i); }
        ArrayStack_Destroy(INT, &ArrayStack_G);
    }
    state.SetComplexityN(state.range(0));
}

// 测试ArrayStack_G出栈性能
static void ArrayStack_G_Pop_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        ArrayStack(INT) ArrayStack_G;
        ArrayStack_Init(INT, &ArrayStack_G);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) { ArrayStack_Push(INT, &ArrayStack_G, i); }
        state.ResumeTiming();
        int Temp;
        for (int i = 0; i < state.range(0); ++i) { ArrayStack_Pop(INT, &ArrayStack_G, &Temp); }
        ArrayStack_Destroy(INT, &ArrayStack_G);
    }
    state.SetComplexityN(state.range(0));
}

// 测试stack入栈性能
static void stack_Push_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::stack<int> stack;
        for (int i = 0; i < state.range(0); ++i) { stack.push(i); }
    }
    state.SetComplexityN(state.range(0));
}

// 测试stack出栈性能
static void stack_Pop_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::stack<int> stack;
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) { stack.push(i); }
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) { stack.pop(); }
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(ArrayStack_G_Push_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
BENCHMARK(stack_Push_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
BENCHMARK(ArrayStack_G_Pop_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);
BENCHMARK(stack_Pop_Test)->RangeMultiplier(10)->Range(START, LIMIT)->Complexity()->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
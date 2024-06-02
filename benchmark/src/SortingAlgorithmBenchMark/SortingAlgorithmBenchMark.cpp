#include <benchmark/benchmark.h>
#include <algorithm>
#include <random>
#include "SortingAlgorithm_G.h"

/*
    Mode == 1 //顺序数组
    Mode == 2 //逆序数组
    Mode == 3 //普通随机数组
    Mode == 4 //无相同元素的乱序数组
    Mode == 5 //有很多相同元素的数组
    Mode == 6 //16个元素的小数组
*/
#define NUMBER       10000
#define Mode         3

#define INTCMP(a, b) (((*a) < (*b)) ? -1 : (((*a) == (*b)) ? 0 : +1))
LTT_SORT_INIT(INT, int, INTCMP);


int Array[NUMBER];

static void GetTheRandomNonRepeatingArray(int* array, int NumOfElements)
{
    for (int i = 0; i < NumOfElements; ++i) { array[i] = i; }

    // 创建随机数生成器
    std::random_device         rd;           // 随机设备种子
    std::default_random_engine eng(rd());    // 随机数引擎
    // 使用随机数引擎对数组进行乱序
    std::shuffle(array, &array[NumOfElements], eng);
}

static void SetArray()
{
#if Mode == 1      // 顺序数组
    for (int i = 0; i < NUMBER; ++i) Array[i] = i;
#elif Mode == 2    // 逆序数组
    for (int i = 0; i < NUMBER; ++i) Array[i] = NUMBER - i;
#elif Mode == 3    // 普通随机数组
    std::random_device                 rd;                      // 使用随机设备种子
    std::default_random_engine         eng(rd());               // 随机数引擎
    std::uniform_int_distribution<int> distr(0, NUMBER - 1);    // 定义分布范围
    for (int& i : Array) i = distr(eng);
#elif Mode == 4    // 无相同元素的乱序数组
    GetTheRandomNonRepeatingArray(Array, NUMBER);
#elif Mode == 5    // 有很多相同元素的数组
    std::random_device                 rd;               // 使用随机设备种子
    std::default_random_engine         eng(rd());        // 随机数引擎
    std::uniform_int_distribution<int> distr(0, 100);    // 定义分布范围
    for (int& i : Array) i = distr(eng);
#elif Mode == 6    // 16个元素的小数组
    #undef NUMBER
    #define NUMBER 16
    GetTheRandomNonRepeatingArray(Array, NUMBER);
#endif
}

// 测试BinaryInsertionSort性能
static void BinaryInsertionSort_Test(benchmark::State& state)
{
    int ArrayCopy[NUMBER];
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        BinaryInsertionSort(INT, ArrayCopy, NUMBER);
    }
}

// 测试ShellInsertionSort_Hibbard性能
static void ShellInsertionSort_Hibbard_Test(benchmark::State& state)
{
    int ArrayCopy[NUMBER];
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        ShellInsertionSort_Hibbard(INT, ArrayCopy, NUMBER);
    }
}

// 测试ShellInsertionSort_Sedgewick性能
static void ShellInsertionSort_Sedgewick_Test(benchmark::State& state)
{
    int ArrayCopy[NUMBER];
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        ShellInsertionSort_Sedgewick(INT, ArrayCopy, NUMBER);
    }
}

// 测试BubbleSort性能
static void BubbleSort_Test(benchmark::State& state)
{
    int ArrayCopy[NUMBER];
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        BubbleSort(INT, ArrayCopy, NUMBER);
    }
}

// 测试BubbleSort_Fast性能
static void BubbleSort_Fast_Test(benchmark::State& state)
{
    int ArrayCopy[NUMBER];
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        BubbleSort_Fast(INT, ArrayCopy, NUMBER);
    }
}

// 测试QuickSort_LTT_glibc性能
static void QuickSort_LTT_glibc_Test(benchmark::State& state)
{
    int ArrayCopy[NUMBER];
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        QuickSort_LTT_glibc(INT, ArrayCopy, NUMBER);
    }
}

// 测试QuickSort_LTT_libstdcpp性能
static void QuickSort_LTT_libstdcpp_Test(benchmark::State& state)
{
    int ArrayCopy[NUMBER];
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        QuickSort_LTT_libstdcpp(INT, ArrayCopy, NUMBER);
    }
}

// 测试QuickSort_glibc性能
static void QuickSort_glibc_Test(benchmark::State& state)
{
    int ArrayCopy[NUMBER];
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        QuickSort_glibc(INT, ArrayCopy, NUMBER);
    }
}

BENCHMARK(BinaryInsertionSort_Test)->Unit(benchmark::kMillisecond);
BENCHMARK(ShellInsertionSort_Hibbard_Test)->Unit(benchmark::kMillisecond);
BENCHMARK(ShellInsertionSort_Sedgewick_Test)->Unit(benchmark::kMillisecond);
BENCHMARK(BubbleSort_Test)->Unit(benchmark::kMillisecond);
BENCHMARK(BubbleSort_Fast_Test)->Unit(benchmark::kMillisecond);
BENCHMARK(QuickSort_glibc_Test)->Unit(benchmark::kMillisecond);
BENCHMARK(QuickSort_LTT_glibc_Test)->Unit(benchmark::kMillisecond);
BENCHMARK(QuickSort_LTT_libstdcpp_Test)->Unit(benchmark::kMillisecond);

int main(int argc, char** argv)
{
    SetArray();    // 设置数组
    char  arg0_default[] = "benchmark";
    char* args_default   = arg0_default;
    if (!argv)
    {
        argc = 1;
        argv = &args_default;
    }
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    return 0;
}
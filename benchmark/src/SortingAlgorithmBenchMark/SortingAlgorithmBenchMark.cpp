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
#define NUMBER                                       1000000
#define Mode                                         3
// kNanosecond kMicrosecond kMillisecond kSecond
#define TIMEUNIT                                     kMillisecond

#define InsertionSort_Test_Add                       0
#define BinaryInsertionSort_Test_Add                 0
#define ShellInsertionSort_Hibbard_Test_Add          0
#define ShellInsertionSort_Sedgewick_Test_Add        0
#define BubbleSort_Test_Add                          0
#define BubbleSort_Fast_Test_Add                     0
#define QuickSort_glibc_Test_Add                     0
#define QuickSort_LTT_libstdcpp_Test_Add             1
#define SimpleSelectionSort_Test_Add                 0
#define MergeSort_Recursion_Test_Add                 0
#define MergeSort_Iterative_Test_Add                 0
#define MergeSort_Inplace_Iterative_Test_Add         0
#define MergeSort_Inplace_Iterative_For_Int_Test_Add 0
#define PigeonholeSort_Test_Add                      1
#define qsort_Test_Add                               0
#define sort_Test_Add                                1



#define INTCMP(a, b)                                 (((*(a)) < (*(b))) ? -1 : (((*(a)) == (*(b))) ? 0 : +1))
LTT_SORT_INIT(INT, int, INTCMP);


int* Array     = new int[NUMBER];
int* ArrayCopy = new int[NUMBER];

static LTT_unused void GetTheRandomNonRepeatingArray(int* array, int NumOfElements)
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
    for (int i = 0; i < NUMBER; ++i) Array[i] = distr(eng);
#elif Mode == 4    // 无相同元素的乱序数组
    GetTheRandomNonRepeatingArray(Array, NUMBER);
#elif Mode == 5    // 有很多相同元素的数组
    std::random_device                 rd;              // 使用随机设备种子
    std::default_random_engine         eng(rd());       // 随机数引擎
    std::uniform_int_distribution<int> distr(0, 10);    // 定义分布范围
    for (int i = 0; i < NUMBER; ++i) Array[i] = distr(eng);
#elif Mode == 6    // 16个元素的小数组
    #undef NUMBER
    #define NUMBER 16
    GetTheRandomNonRepeatingArray(Array, NUMBER);
#endif
}

// 测试InsertionSort性能
static LTT_unused void InsertionSort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        InsertionSort(INT, ArrayCopy, NUMBER);
    }
}

// 测试BinaryInsertionSort性能
static LTT_unused void BinaryInsertionSort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        BinaryInsertionSort(INT, ArrayCopy, NUMBER);
    }
}

// 测试ShellInsertionSort_Hibbard性能
static LTT_unused void ShellInsertionSort_Hibbard_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        ShellInsertionSort_Hibbard(INT, ArrayCopy, NUMBER);
    }
}

// 测试ShellInsertionSort_Sedgewick性能
static LTT_unused void ShellInsertionSort_Sedgewick_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        ShellInsertionSort_Sedgewick(INT, ArrayCopy, NUMBER);
    }
}

// 测试BubbleSort性能
static LTT_unused void BubbleSort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        BubbleSort(INT, ArrayCopy, NUMBER);
    }
}

// 测试BubbleSort_Fast性能
static LTT_unused void BubbleSort_Fast_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        BubbleSort_Fast(INT, ArrayCopy, NUMBER);
    }
}

// 测试QuickSort_glibc性能
static LTT_unused void QuickSort_glibc_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        QuickSort_glibc(INT, ArrayCopy, NUMBER);
    }
}

// 测试QuickSort_LTT_libstdcpp性能
static LTT_unused void QuickSort_LTT_libstdcpp_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        QuickSort_LTT_libstdcpp(INT, ArrayCopy, NUMBER);
    }
}

// 测试SimpleSelectionSort性能
static LTT_unused void SimpleSelectionSort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        SimpleSelectionSort(INT, ArrayCopy, NUMBER);
    }
}

// 测试MergeSort_Recursion性能
static LTT_unused void MergeSort_Recursion_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        MergeSort_Recursion(INT, ArrayCopy, NUMBER);
    }
}

// 测试MergeSort_Iterative性能
static LTT_unused void MergeSort_Iterative_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        MergeSort_Iterative(INT, ArrayCopy, NUMBER);
    }
}

// 测试MergeSort_Inplace_Iterative性能
static LTT_unused void MergeSort_Inplace_Iterative_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        MergeSort_Inplace_Iterative(INT, ArrayCopy, NUMBER);
    }
}

// 测试MergeSort_Inplace_Iterative_For_Int性能
static LTT_unused void MergeSort_Inplace_Iterative_For_Int_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        MergeSort_Inplace_Iterative_For_Int(ArrayCopy, NUMBER);
    }
}

// 测试PigeonholeSort性能
static LTT_unused void PigeonholeSort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        PigeonholeSort(ArrayCopy, 0, NUMBER - 1, NUMBER);
    }
}

// 测试qsort性能
static LTT_unused void qsort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        qsort(ArrayCopy, NUMBER, sizeof(int), [](const void* a, const void* b) -> int { return (*(int*)a - *(int*)b); });
    }
}

// 测试sort性能
static LTT_unused void sort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBER, ArrayCopy);
        state.ResumeTiming();
        std::sort(ArrayCopy, ArrayCopy + NUMBER);
    }
}

#if InsertionSort_Test_Add == 1
BENCHMARK(InsertionSort_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if BinaryInsertionSort_Test_Add == 1
BENCHMARK(BinaryInsertionSort_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if ShellInsertionSort_Hibbard_Test_Add == 1
BENCHMARK(ShellInsertionSort_Hibbard_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if ShellInsertionSort_Sedgewick_Test_Add == 1
BENCHMARK(ShellInsertionSort_Sedgewick_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if BubbleSort_Test_Add == 1
BENCHMARK(BubbleSort_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if BubbleSort_Fast_Test_Add == 1
BENCHMARK(BubbleSort_Fast_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if QuickSort_glibc_Test_Add == 1
BENCHMARK(QuickSort_glibc_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if QuickSort_LTT_libstdcpp_Test_Add == 1
BENCHMARK(QuickSort_LTT_libstdcpp_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if SimpleSelectionSort_Test_Add == 1
BENCHMARK(SimpleSelectionSort_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if MergeSort_Recursion_Test_Add == 1
BENCHMARK(MergeSort_Recursion_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if MergeSort_Iterative_Test_Add == 1
BENCHMARK(MergeSort_Iterative_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if MergeSort_Inplace_Iterative_Test_Add == 1
BENCHMARK(MergeSort_Inplace_Iterative_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if MergeSort_Inplace_Iterative_For_Int_Test_Add == 1
BENCHMARK(MergeSort_Inplace_Iterative_For_Int_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if PigeonholeSort_Test_Add == 1
BENCHMARK(PigeonholeSort_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if qsort_Test_Add == 1
BENCHMARK(qsort_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if sort_Test_Add == 1
BENCHMARK(sort_Test)->Unit(benchmark::TIMEUNIT);
#endif

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
#include "SortingAlgorithm_G.h"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <random>

/*
    Mode == 1 //顺序数组
    Mode == 2 //逆序数组
    Mode == 3 //普通随机数组
    Mode == 4 //无相同元素的乱序数组
    Mode == 5 //有很多相同元素的数组
    Mode == 6 //16个元素的小数组
*/
#define Mode    3
#define NUMBERS 1000000
static int Min;
static int Max;
// kNanosecond kMicrosecond kMillisecond kSecond
#define TIMEUNIT                                     kMillisecond

#define InsertionSort_Test_Add                       0
#define BinaryInsertionSort_Test_Add                 0
#define ShellInsertionSort_Hibbard_Test_Add          0
#define ShellInsertionSort_Sedgewick_Test_Add        0
#define BubbleSort_Test_Add                          0
#define CockTailSort_Test_Add                        0
#define QuickSort_glibc_Test_Add                     1
#define QuickSort_LTT_libstdcpp_Test_Add             1
#define SimpleSelectionSort_Test_Add                 0
#define MergeSort_Recursion_Test_Add                 0
#define MergeSort_Iterative_Test_Add                 0
#define MergeSort_Inplace_Iterative_Test_Add         0
#define MergeSort_Inplace_Iterative_For_Int_Test_Add 0
#define PigeonholeSort_Test_Add                      0
#define CountingSort_Test_Add                        1
#define TallySort_Test_Add                           1
#define qsort_Test_Add                               1
#define sort_Test_Add                                1



#define INTCMP(a, b)                                 (((*(a)) < (*(b))) ? -1 : (((*(a)) == (*(b))) ? 0 : +1))
LTT_SORT_CMP_INIT(INT, int, INTCMP);

#define INTKEY(a) (*(a))
LTT_SORT_INT_INIT(INT, int, INTKEY);

int* Array     = new int[NUMBERS];
int* ArrayCopy = new int[NUMBERS];

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
    for (int i = 0; i < NUMBERS; ++i) Array[i] = i;
    Min = 0;
    Max = NUMBERS - 1;
#elif Mode == 2    // 逆序数组
    for (int i = 0; i < NUMBERS; ++i) Array[i] = NUMBERS - i - 1;
    Min = 0;
    Max = NUMBERS - 1;
#elif Mode == 3    // 普通随机数组
    std::random_device                 rd;                       // 使用随机设备种子
    std::default_random_engine         eng(rd());                // 随机数引擎
    std::uniform_int_distribution<int> distr(0, NUMBERS - 1);    // 定义分布范围
    for (int i = 0; i < NUMBERS; ++i) Array[i] = distr(eng);
    Min = 0;
    Max = NUMBERS - 1;
#elif Mode == 4    // 无相同元素的乱序数组
    GetTheRandomNonRepeatingArray(Array, NUMBERS);
    Min = 0;
    Max = NUMBERS - 1;
#elif Mode == 5    // 有很多相同元素的数组
    std::random_device                 rd;               // 使用随机设备种子
    std::default_random_engine         eng(rd());        // 随机数引擎
    std::uniform_int_distribution<int> distr(0, 100);    // 定义分布范围
    for (int i = 0; i < NUMBERS; ++i) Array[i] = distr(eng);
    Min = 0;
    Max = 100;
#elif Mode == 6    // 16个元素的小数组
    #undef NUMBERS
    #define NUMBERS 16
    GetTheRandomNonRepeatingArray(Array, NUMBERS);
    Min = 0;
    Max = NUMBERS - 1;
#endif
}

// 测试InsertionSort性能
static LTT_unused void InsertionSort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        InsertionSort(INT, ArrayCopy, NUMBERS);
    }
}

// 测试BinaryInsertionSort性能
static LTT_unused void BinaryInsertionSort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        BinaryInsertionSort(INT, ArrayCopy, NUMBERS);
    }
}

// 测试ShellInsertionSort_Hibbard性能
static LTT_unused void ShellInsertionSort_Hibbard_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        ShellInsertionSort_Hibbard(INT, ArrayCopy, NUMBERS);
    }
}

// 测试ShellInsertionSort_Sedgewick性能
static LTT_unused void ShellInsertionSort_Sedgewick_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        ShellInsertionSort_Sedgewick(INT, ArrayCopy, NUMBERS);
    }
}

// 测试BubbleSort性能
static LTT_unused void BubbleSort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        BubbleSort(INT, ArrayCopy, NUMBERS);
    }
}

// 测试CockTailSort性能
static LTT_unused void CockTailSort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        CockTailSort(INT, ArrayCopy, NUMBERS);
    }
}

// 测试QuickSort_glibc性能
static LTT_unused void QuickSort_glibc_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        QuickSort_glibc(INT, ArrayCopy, NUMBERS);
    }
}

// 测试QuickSort_LTT_libstdcpp性能
static LTT_unused void QuickSort_LTT_libstdcpp_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        QuickSort_LTT_libstdcpp(INT, ArrayCopy, NUMBERS);
    }
}

// 测试SimpleSelectionSort性能
static LTT_unused void SimpleSelectionSort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        SimpleSelectionSort(INT, ArrayCopy, NUMBERS);
    }
}

// 测试MergeSort_Recursion性能
static LTT_unused void MergeSort_Recursion_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        MergeSort_Recursion(INT, ArrayCopy, NUMBERS);
    }
}

// 测试MergeSort_Iterative性能
static LTT_unused void MergeSort_Iterative_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        MergeSort_Iterative(INT, ArrayCopy, NUMBERS);
    }
}

// 测试MergeSort_Inplace_Iterative性能
static LTT_unused void MergeSort_Inplace_Iterative_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        MergeSort_Inplace_Iterative(INT, ArrayCopy, NUMBERS);
    }
}

// 测试MergeSort_Inplace_Iterative_For_Int性能
static LTT_unused void MergeSort_Inplace_Iterative_For_Int_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        MergeSort_Inplace_Iterative_For_Int(ArrayCopy, NUMBERS);
    }
}

// 测试PigeonholeSort性能
static LTT_unused void PigeonholeSort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        PigeonholeSort(INT, ArrayCopy, Min, Max, NUMBERS);
    }
}

// 测试CountingSort性能
static LTT_unused void CountingSort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        CountingSort(INT, ArrayCopy, Min, Max, NUMBERS);
    }
}

// 测试TallySort性能
static LTT_unused void TallySort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        TallySort(ArrayCopy, Min, Max, NUMBERS);
    }
}

// 测试qsort性能
static LTT_unused void qsort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        qsort(ArrayCopy, NUMBERS, sizeof(int), [](const void* a, const void* b) -> int
        { return (*(int*)a - *(int*)b); });
    }
}

// 测试sort性能
static LTT_unused void sort_Test(benchmark::State& state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::copy(Array, Array + NUMBERS, ArrayCopy);
        state.ResumeTiming();
        std::sort(ArrayCopy, ArrayCopy + NUMBERS);
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
#if CockTailSort_Test_Add == 1
BENCHMARK(CockTailSort_Test)->Unit(benchmark::TIMEUNIT);
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
#if CountingSort_Test_Add == 1
BENCHMARK(CountingSort_Test)->Unit(benchmark::TIMEUNIT);
#endif
#if TallySort_Test_Add == 1
BENCHMARK(TallySort_Test)->Unit(benchmark::TIMEUNIT);
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
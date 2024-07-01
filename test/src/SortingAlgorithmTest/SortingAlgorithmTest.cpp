#include <gtest/gtest.h>
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
#define NUMBER 10000
static int Min;
static int Max;
#define Mode         3

// 定义比较函数
#define INTCMP(a, b) (((*(a)) < (*(b))) ? -1 : (((*(a)) == (*(b))) ? 0 : +1))
LTT_SORT_CMP_INIT(INT, int, INTCMP);

// 定义整数键值函数
#define INTKEY(a) (*(a))
LTT_SORT_INT_INIT(INT, int, INTKEY);

class SortingAlgorithmTest: public ::testing::Test
{
protected:
    int Array[NUMBER] {};
    int OrderedArray[NUMBER] {};

    static void GetTheRandomNonRepeatingArray(int* array, int NumOfElements)
    {
        for (int i = 0; i < NumOfElements; ++i) { array[i] = i; }

        // 创建随机数生成器
        std::random_device         rd;           // 随机设备种子
        std::default_random_engine eng(rd());    // 随机数引擎
        // 使用随机数引擎对数组进行乱序
        std::shuffle(array, &array[NumOfElements], eng);
    }

    void SetUp() override
    {
#if Mode == 1                                              // 顺序数组
        for (int i = 0; i < NUMBER; ++i) Array[i] = i;
        Min = 0;
        Max = NUMBER - 1;
#elif Mode == 2                                            // 逆序数组
        for (int i = 0; i < NUMBER; ++i) Array[i] = NUMBER - i - 1;
        Min = 0;
        Max = NUMBER - 1;
#elif Mode == 3                                            // 普通随机数组
        std::random_device                 rd;                      // 使用随机设备种子
        std::default_random_engine         eng(rd());               // 随机数引擎
        std::uniform_int_distribution<int> distr(0, NUMBER - 1);    // 定义分布范围
        for (int& i : Array) i = distr(eng);
        Min = 0;
        Max = NUMBER - 1;
#elif Mode == 4                                            // 无相同元素的乱序数组
        GetTheRandomNonRepeatingArray(Array, NUMBER);
        Min = 0;
        Max = NUMBER - 1;
#elif Mode == 5                                            // 有很多相同元素的数组
        std::random_device                 rd;               // 使用随机设备种子
        std::default_random_engine         eng(rd());        // 随机数引擎
        std::uniform_int_distribution<int> distr(0, 100);    // 定义分布范围
        for (int& i : Array) i = distr(eng);
        Min = 0;
        Max = 100;
#elif Mode == 6                                            // 16个元素的小数组
    #undef NUMBER
    #define NUMBER 16
        GetTheRandomNonRepeatingArray(Array, NUMBER);
        Min = 0;
        Max = NUMBER - 1;
#endif
        std::copy(Array, Array + NUMBER, OrderedArray);    // 复制Array到OrderedArray
        std::sort(OrderedArray, OrderedArray + NUMBER);    // 对OrderedArray进行排序
    }
};

// 测试InsertionSort
TEST_F(SortingAlgorithmTest, InsertionSort)
{
    InsertionSort(INT, Array, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试BinaryInsertionSort
TEST_F(SortingAlgorithmTest, BinaryInsertionSort)
{
    BinaryInsertionSort(INT, Array, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试ShellInsertionSort_Hibbard
TEST_F(SortingAlgorithmTest, ShellInsertionSort_Hibbard)
{
    ShellInsertionSort_Hibbard(INT, Array, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试ShellInsertionSort_Sedgewick
TEST_F(SortingAlgorithmTest, ShellInsertionSort_Sedgewick)
{
    ShellInsertionSort_Sedgewick(INT, Array, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试BubbleSort
TEST_F(SortingAlgorithmTest, BubbleSort)
{
    BubbleSort(INT, Array, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试CockTailSort
TEST_F(SortingAlgorithmTest, CockTailSort)
{
    CockTailSort(INT, Array, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试QuickSort_glibc
TEST_F(SortingAlgorithmTest, QuickSort_glibc)
{
    QuickSort_glibc(INT, Array, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试QuickSort_LTT_libstdcpp
TEST_F(SortingAlgorithmTest, QuickSort_LTT_libstdcpp)
{
    QuickSort_LTT_libstdcpp(INT, Array, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试SimpleSelectionSort
TEST_F(SortingAlgorithmTest, SimpleSelectionSort)
{
    SimpleSelectionSort(INT, Array, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试MergeSort_Recursion
TEST_F(SortingAlgorithmTest, MergeSort_Recursion)
{
    MergeSort_Recursion(INT, Array, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试MergeSort_Iterative
TEST_F(SortingAlgorithmTest, MergeSort_Iterative)
{
    MergeSort_Iterative(INT, Array, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试MergeSort_Inplace_Iterative
TEST_F(SortingAlgorithmTest, MergeSort_Inplace_Iterative)
{
    MergeSort_Inplace_Iterative(INT, Array, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试MergeSort_Inplace_Iterative_For_Int
TEST_F(SortingAlgorithmTest, MergeSort_Inplace_Iterative_For_Int)
{
    MergeSort_Inplace_Iterative_For_Int(Array, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试PigeonholeSort
TEST_F(SortingAlgorithmTest, PigeonholeSort)
{
    PigeonholeSort(INT, Array, Min, Max, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试CountingSort
TEST_F(SortingAlgorithmTest, CountingSort)
{
    CountingSort(INT, Array, Min, Max, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}

// 测试TallySor
TEST_F(SortingAlgorithmTest, TallySor)
{
    TallySort(Array, Min, Max, NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(Array[i], OrderedArray[i]); }
}
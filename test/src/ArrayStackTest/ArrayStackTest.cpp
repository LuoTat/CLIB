#include "ArrayStack_G.h"
#include <gtest/gtest.h>

// 定义比较函数
#define INTCMP(a, b) ((a) == (b))

// 定义元素个数
#define NUMBER       10000

// 初始化ArrayStack类型
LTT_ARRAYSTACK_INIT(INT, int, INTCMP);

class ArrayStackTest: public ::testing::Test
{
protected:
    ArrayStack(INT) ArrayStack_G;

    void SetUp() override
    {
        ArrayStack_Init(INT, &ArrayStack_G);
        EXPECT_EQ(ArrayStack_GetSize(&ArrayStack_G), 0);
        EXPECT_TRUE(ArrayStack_IsEmpty(&ArrayStack_G));
    }

    void TearDown() override
    {
        ArrayStack_Destroy(INT, &ArrayStack_G);
        EXPECT_EQ(ArrayStack_GetSize(&ArrayStack_G), 0);
        EXPECT_TRUE(ArrayStack_IsEmpty(&ArrayStack_G));
        EXPECT_EQ(ArrayStack_G.Array, nullptr);
    }

    // 默认Push Number个元素
    void PushDefault(std::vector<int>& Array)
    {
        for (int i = 0; i < NUMBER; ++i) { Array.push_back(i); }
        PushFromArray(Array);
    }

    // 默认Pop Number个元素
    void PopDefault(int* Array) { PopToArray(Array, NUMBER); }

    // 将Array的元素AddFirst到ArrayStack_G
    void PushFromArray(std::vector<int>& Array)
    {
        for (auto i : Array) { EXPECT_EQ(ArrayStack_Push(INT, &ArrayStack_G, i), Success); }
    }

    // 从ArrayStack_G DeleteFirst Number个元素到Array中
    void PopToArray(int* Array, int Number)
    {
        for (int i = 0; i < Number; ++i) { EXPECT_EQ(ArrayStack_Pop(INT, &ArrayStack_G, &Array[i]), Success); }
    }
};

// 测试Push
TEST_F(ArrayStackTest, Push)
{
    std::vector<int> Array;
    PushDefault(Array);
    // 逐个比较
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(ArrayStack_G.Array[i], Array[i]); }
    // Size是否正确
    EXPECT_EQ(ArrayStack_GetSize(&ArrayStack_G), NUMBER);
    Array.clear();
}

// 测试Pop
TEST_F(ArrayStackTest, Pop)
{
    std::vector<int> InArray;
    PushDefault(InArray);
    int* OutArray = new int[NUMBER];
    PopDefault(OutArray);
    // 逐个比较
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(OutArray[i], InArray[NUMBER - i - 1]); }
    // Size是否正确
    EXPECT_EQ(ArrayStack_GetSize(&ArrayStack_G), 0);
    // 是否为空
    EXPECT_TRUE(ArrayStack_IsEmpty(&ArrayStack_G));
    InArray.clear();
    delete[] OutArray;
}

// 测试Peek
TEST_F(ArrayStackTest, Peek)
{
    std::vector<int> Array;
    PushDefault(Array);
    int result;
    EXPECT_EQ(ArrayStack_Peek(INT, &ArrayStack_G, &result), Success);
    EXPECT_EQ(result, Array[NUMBER - 1]);
    Array.clear();
}

// 测试GetSize
TEST_F(ArrayStackTest, GetSize)
{
    std::vector<int> Array;
    PushDefault(Array);
    EXPECT_EQ(ArrayStack_GetSize(&ArrayStack_G), NUMBER);
    Array.clear();
}

// 测试IsEmpty
TEST_F(ArrayStackTest, IsEmpty)
{
    EXPECT_TRUE(ArrayStack_IsEmpty(&ArrayStack_G));
    std::vector<int> Array;
    PushDefault(Array);
    EXPECT_FALSE(ArrayStack_IsEmpty(&ArrayStack_G));
    Array.clear();
}

// 测试Contains
TEST_F(ArrayStackTest, Contains)
{
    std::vector<int> Array;
    PushDefault(Array);
    EXPECT_TRUE(ArrayStack_Contains(INT, &ArrayStack_G, NUMBER / 2));
    Array.clear();
}

// 测试Clear
TEST_F(ArrayStackTest, Clear)
{
    std::vector<int> Array;
    PushDefault(Array);
    ArrayStack_Clear(&ArrayStack_G);
    EXPECT_EQ(ArrayStack_GetSize(&ArrayStack_G), 0);
    EXPECT_TRUE(ArrayStack_IsEmpty(&ArrayStack_G));
    Array.clear();
}

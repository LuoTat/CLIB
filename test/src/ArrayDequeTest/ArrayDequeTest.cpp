#include <gtest/gtest.h>
#include <deque>
#include "ArrayDeque_G.h"

// 定义比较函数
#define INTCMP(a, b) ((a) == (b))

// 定义元素个数
#define NUMBER       10000

// 初始化ArrayList类型
LTT_ARRAYDEQUE_INIT(INT, int, INTCMP);

class ArrayListTest: public ::testing::Test
{
protected:
    ArrayDeque(INT) ArrayDeque_G;

    void SetUp() override
    {
        ArrayDeque_Init(INT, &ArrayDeque_G);
        EXPECT_EQ(ArrayDeque_GetSize(&ArrayDeque_G), 0);
        EXPECT_TRUE(ArrayDeque_IsEmpty(&ArrayDeque_G));
    }

    void TearDown() override
    {
        ArrayDeque_Destroy(INT, &ArrayDeque_G);
        EXPECT_EQ(ArrayDeque_GetSize(&ArrayDeque_G), 0);
        EXPECT_TRUE(ArrayDeque_IsEmpty(&ArrayDeque_G));
        EXPECT_EQ(ArrayDeque_G.Array, nullptr);
    }

    // 默认AddFirst Number个元素
    void AddFirstDefault(std::deque<int>& Array)
    {
        for (int i = 0; i < NUMBER; ++i) { Array.push_back(i); }
        AddFirstFromArray(Array);
    }

    // 默认AddLast Number个元素
    void AddLastDefault(std::deque<int>& Array)
    {
        for (int i = 0; i < NUMBER; ++i) { Array.push_back(i); }
        AddLastFromArray(Array);
    }

    // 默认DeleteFirst Number个元素
    void DeleteFirstDefault(int* Array) { DeleteFirstToArray(Array, NUMBER); }

    // 默认DeleteLast Number个元素
    void DeleteLastDefault(int* Array) { DeleteLastToArray(Array, NUMBER); }

    // 将Array的元素AddFirst到ArrayDeque_G
    void AddFirstFromArray(std::deque<int>& Array)
    {
        for (auto i : Array) { EXPECT_EQ(ArrayDeque_AddFirst(INT, &ArrayDeque_G, i), Success); }
    }

    // 将Array的元素AddLast到ArrayDeque_G
    void AddLastFromArray(std::deque<int>& Array)
    {
        for (auto i : Array) { EXPECT_EQ(ArrayDeque_AddLast(INT, &ArrayDeque_G, i), Success); }
    }

    // 从ArrayDeque_G DeleteFirst Number个元素到Array中
    void DeleteFirstToArray(int* Array, int Number)
    {
        for (int i = 0; i < Number; ++i) { EXPECT_EQ(ArrayDeque_DeleteFirst(INT, &ArrayDeque_G, &Array[i]), Success); }
    }

    // 从ArrayDeque_G DeleteLast Number个元素到Array中
    void DeleteLastToArray(int* Array, int Number)
    {
        for (int i = 0; i < Number; ++i) { EXPECT_EQ(ArrayDeque_DeleteLast(INT, &ArrayDeque_G, &Array[i]), Success); }
    }

    // 同余类+1
    static int INC(int Head, int Capacity) { return (++Head >= Capacity) ? 0 : Head; }

    // 同余类-1
    static int DEC(int Head, int Capacity) { return (--Head < 0) ? Capacity - 1 : Head; }
};

// 测试AddFirst
TEST_F(ArrayListTest, AddFirst)
{
    std::deque<int> Array;
    AddFirstDefault(Array);
    // 逐个比较
    int index = ArrayDeque_G.Head;
    for (int i = 0; i < NUMBER; ++i)
    {
        EXPECT_EQ(ArrayDeque_G.Array[index], Array[NUMBER - i - 1]);
        index = INC(index, ArrayDeque_G.Capacity);
    }
    // Size是否正确
    EXPECT_EQ(ArrayDeque_GetSize(&ArrayDeque_G), NUMBER);
    Array.clear();
}

// 测试AddLast
TEST_F(ArrayListTest, AddLast)
{
    std::deque<int> Array;
    AddLastDefault(Array);
    // 逐个比较
    int index = ArrayDeque_G.Head;
    for (int i = 0; i < NUMBER; ++i)
    {
        EXPECT_EQ(ArrayDeque_G.Array[index], Array[i]);
        index = INC(index, ArrayDeque_G.Capacity);
    }
    // Size是否正确
    EXPECT_EQ(ArrayDeque_GetSize(&ArrayDeque_G), NUMBER);
    Array.clear();
}

// 测试DeleteFirst
TEST_F(ArrayListTest, DeleteFirst)
{
    std::deque<int> InArray;
    AddLastDefault(InArray);
    int* OutArray = new int[NUMBER];
    DeleteFirstDefault(OutArray);
    // 逐个比较
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(OutArray[i], InArray[i]); }
    // Size是否正确
    EXPECT_EQ(ArrayDeque_GetSize(&ArrayDeque_G), 0);
    // 是否为空
    EXPECT_TRUE(ArrayDeque_IsEmpty(&ArrayDeque_G));
    InArray.clear();
    delete[] OutArray;
}

// 测试DeleteLast
TEST_F(ArrayListTest, DeleteLast)
{
    std::deque<int> InArray;
    AddLastDefault(InArray);
    int* OutArray = new int[NUMBER];
    DeleteLastDefault(OutArray);
    // 逐个比较
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(OutArray[i], InArray[NUMBER - i - 1]); }
    // Size是否正确
    EXPECT_EQ(ArrayDeque_GetSize(&ArrayDeque_G), 0);
    // 是否为空
    EXPECT_TRUE(ArrayDeque_IsEmpty(&ArrayDeque_G));
    InArray.clear();
    delete[] OutArray;
}

// 测试GetFirst
TEST_F(ArrayListTest, GetFirst)
{
    std::deque<int> Array;
    AddLastDefault(Array);
    int result;
    EXPECT_EQ(ArrayDeque_GetFirst(INT, &ArrayDeque_G, &result), Success);
    EXPECT_EQ(result, Array[0]);
    Array.clear();
}

// 测试GetLast
TEST_F(ArrayListTest, GetLast)
{
    std::deque<int> Array;
    AddLastDefault(Array);
    int result;
    EXPECT_EQ(ArrayDeque_GetLast(INT, &ArrayDeque_G, &result), Success);
    EXPECT_EQ(result, Array[NUMBER - 1]);
    Array.clear();
}

// 测试GetSize
TEST_F(ArrayListTest, GetSize)
{
    std::deque<int> Array;
    AddLastDefault(Array);
    EXPECT_EQ(ArrayDeque_GetSize(&ArrayDeque_G), NUMBER);
    Array.clear();
}

// 测试IsEmpty
TEST_F(ArrayListTest, IsEmpty)
{
    EXPECT_TRUE(ArrayDeque_IsEmpty(&ArrayDeque_G));
    std::deque<int> Array;
    AddLastDefault(Array);
    EXPECT_FALSE(ArrayDeque_IsEmpty(&ArrayDeque_G));
    Array.clear();
}

// 测试Contains
TEST_F(ArrayListTest, Contains)
{
    std::deque<int> Array;
    AddLastDefault(Array);
    EXPECT_TRUE(ArrayDeque_Contains(INT, &ArrayDeque_G, NUMBER / 2));
    Array.clear();
}

// 测试Clear
TEST_F(ArrayListTest, Clear)
{
    std::deque<int> Array;
    AddLastDefault(Array);
    ArrayDeque_Clear(&ArrayDeque_G);
    EXPECT_EQ(ArrayDeque_GetSize(&ArrayDeque_G), 0);
    EXPECT_TRUE(ArrayDeque_IsEmpty(&ArrayDeque_G));
    Array.clear();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

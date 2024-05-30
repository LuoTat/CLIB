#include <gtest/gtest.h>
#include <vector>
#include "ArrayList_G.h"

// 定义比较函数
#define INTCMP(a, b) ((a) == (b))

// 定义元素个数
#define NUMBER       100000

// 初始化数组列表类型
LTT_ARRAYLIST_INIT(Int, int, INTCMP);

class ArrayListTest: public ::testing::Test
{
protected:
    ArrayList(Int) ArrayList_G;

    void SetUp() override { ArrayList_Init(Int, &ArrayList_G); }

    void TearDown() override { ArrayList_Destroy(Int, &ArrayList_G); }

    // 默认AddFirst Number个元素
    void AddFirstDefault(std::vector<int>& Array)
    {
        for (int i = 0; i < NUMBER; ++i) { Array.push_back(i); }
        AddFirstFromArray(Array);
    }

    // 默认AddLast Number个元素
    void AddLastDefault(std::vector<int>& Array)
    {
        for (int i = 0; i < NUMBER; ++i) { Array.push_back(i); }
        AddLastFromArray(Array);
    }

    // 默认DeleteFirst Number个元素
    void DeleteFirstDefault(int* Array) { DeleteFirstToArray(Array, NUMBER); }

    // 默认DeleteLast Number个元素
    void DeleteLastDefault(int* Array) { DeleteLastToArray(Array, NUMBER); }

    // 将Array的元素AddFirst到ArrayList_G
    void AddFirstFromArray(std::vector<int>& Array)
    {
        for (auto i : Array) { EXPECT_EQ(ArrayList_AddFirst(Int, &ArrayList_G, i), Success); }
    }

    // 将Array的元素AddLast到ArrayList_G
    void AddLastFromArray(std::vector<int>& Array)
    {
        for (auto i : Array) { EXPECT_EQ(ArrayList_AddLast(Int, &ArrayList_G, i), Success); }
    }

    // 从ArrayList_G DeleteFirst Number个元素到Array中
    void DeleteFirstToArray(int* Array, int Number)
    {
        for (int i = 0; i < Number; ++i) { EXPECT_EQ(ArrayList_DeleteFirst(Int, &ArrayList_G, &Array[i]), Success); }
    }

    // 从ArrayList_G DeleteLast Number个元素到Array中
    void DeleteLastToArray(int* Array, int Number)
    {
        for (int i = 0; i < Number; ++i) { EXPECT_EQ(ArrayList_DeleteLast(Int, &ArrayList_G, &Array[i]), Success); }
    }
};

// 测试初始化
TEST_F(ArrayListTest, Initialization)
{
    EXPECT_EQ(ArrayList_GetSize(&ArrayList_G), 0);
    EXPECT_TRUE(ArrayList_isEmpty(&ArrayList_G));
}

// 测试AddFirst
TEST_F(ArrayListTest, AddFirst)
{
    std::vector<int> Array;
    AddFirstDefault(Array);
    // 逐个比较
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(ArrayList_G.Array[i], Array[NUMBER - i - 1]); }
    // Size是否正确
    EXPECT_EQ(ArrayList_GetSize(&ArrayList_G), NUMBER);
    Array.clear();
}

// 测试AddLast
TEST_F(ArrayListTest, AddLast)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    // 逐个比较
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(ArrayList_G.Array[i], Array[i]); }
    // Size是否正确
    EXPECT_EQ(ArrayList_GetSize(&ArrayList_G), NUMBER);
    Array.clear();
}

// 测试AddIndex
TEST_F(ArrayListTest, AddIndex)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    // 在Index处插入0
    int Index = NUMBER / 2;
    EXPECT_EQ(ArrayList_AddIndex(Int, &ArrayList_G, 0, Index), Success);
    // 逐个从0到Index-1比较
    for (int i = 0; i < Index; ++i) { EXPECT_EQ(ArrayList_G.Array[i], Array[i]); }
    // Index是否为0
    EXPECT_EQ(ArrayList_G.Array[Index], 0);
    // 逐个从Index+1到NUMBER+1比较
    for (int i = Index + 1; i < NUMBER + 1; ++i) { EXPECT_EQ(ArrayList_G.Array[i], Array[i - 1]); }
    // Size是否正确
    EXPECT_EQ(ArrayList_GetSize(&ArrayList_G), NUMBER + 1);
    Array.clear();
}

// 测试DeleteFirst
TEST_F(ArrayListTest, DeleteFirst)
{
    std::vector<int> InArray;
    AddLastDefault(InArray);
    int* OutArray = new int[NUMBER];
    DeleteFirstDefault(OutArray);
    // 逐个比较
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(OutArray[i], InArray[i]); }
    // Size是否正确
    EXPECT_EQ(ArrayList_GetSize(&ArrayList_G), 0);
    //  是否为空
    EXPECT_TRUE(ArrayList_isEmpty(&ArrayList_G));
    InArray.clear();
    delete[] OutArray;
}

// 测试DeleteLast
TEST_F(ArrayListTest, DeleteLast)
{
    std::vector<int> InArray;
    AddLastDefault(InArray);
    int* OutArray = new int[NUMBER];
    DeleteLastDefault(OutArray);
    // 逐个比较
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(OutArray[i], InArray[NUMBER - i - 1]); }
    // Size是否正确
    EXPECT_EQ(ArrayList_GetSize(&ArrayList_G), 0);
    // 是否为空
    EXPECT_TRUE(ArrayList_isEmpty(&ArrayList_G));
    InArray.clear();
    delete[] OutArray;
}

// 测试DeleteIndex
TEST_F(ArrayListTest, DeleteIndex)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    int result;
    int Index = NUMBER / 2;
    // 删除Index处的元素
    EXPECT_EQ(ArrayList_DeleteIndex(Int, &ArrayList_G, Index, &result), Success);
    // Index处的返回的原元素是否正确
    EXPECT_EQ(result, Array[Index]);
    // 逐个比较从0到Index-1
    for (int i = 0; i < Index; ++i) { EXPECT_EQ(ArrayList_G.Array[i], Array[i]); }
    // 逐个比较从Index到NUMBER-1
    for (int i = Index; i < NUMBER - 1; ++i) { EXPECT_EQ(ArrayList_G.Array[i], Array[i + 1]); }
    Array.clear();
}

// 测试SetIndex
TEST_F(ArrayListTest, SetIndex)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    int result;
    int Index = NUMBER / 2;
    EXPECT_EQ(ArrayList_SetIndex(Int, &ArrayList_G, 0, Index, &result), Success);
    // Index处的返回的原元素是否正确
    EXPECT_EQ(result, Array[Index]);
    for (int i = 0; i < Index; ++i) { EXPECT_EQ(ArrayList_G.Array[i], Array[i]); }
    EXPECT_EQ(ArrayList_G.Array[Index], 0);
    for (int i = Index + 1; i < NUMBER; ++i) { EXPECT_EQ(ArrayList_G.Array[i], Array[i]); }
    Array.clear();
}

// 测试GetFirst
TEST_F(ArrayListTest, GetFirst)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    int result;
    EXPECT_EQ(ArrayList_GetFirst(Int, &ArrayList_G, &result), Success);
    EXPECT_EQ(result, Array[0]);
    Array.clear();
}

// 测试GetLast
TEST_F(ArrayListTest, GetLast)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    int result;
    EXPECT_EQ(ArrayList_GetLast(Int, &ArrayList_G, &result), Success);
    EXPECT_EQ(result, Array[NUMBER - 1]);
    Array.clear();
}

// 测试GetIndex
TEST_F(ArrayListTest, GetIndex)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    int result;
    int Index = NUMBER / 2;
    EXPECT_EQ(ArrayList_GetIndex(Int, &ArrayList_G, Index, &result), Success);
    EXPECT_EQ(result, Array[Index]);
    Array.clear();
}

// 测试IndexOf
TEST_F(ArrayListTest, IndexOf)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    int Temp;
    // 将首尾元素设置为0
    EXPECT_EQ(ArrayList_SetIndex(Int, &ArrayList_G, 0, NUMBER - 1, &Temp), Success);
    EXPECT_EQ(ArrayList_IndexOf(Int, &ArrayList_G, 0), 0);
    Array.clear();
}

// 测试LastIndexOf
TEST_F(ArrayListTest, LastIndexOf)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    int Temp;
    // 将首尾元素设置为0
    EXPECT_EQ(ArrayList_SetIndex(Int, &ArrayList_G, 0, NUMBER - 1, &Temp), Success);
    EXPECT_EQ(ArrayList_LastIndexOf(Int, &ArrayList_G, 0), NUMBER - 1);
    Array.clear();
}

// 测试GetSize
TEST_F(ArrayListTest, GetSize)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    EXPECT_EQ(ArrayList_GetSize(&ArrayList_G), NUMBER);
    Array.clear();
}

// 测试IsEmpty
TEST_F(ArrayListTest, IsEmpty)
{
    EXPECT_TRUE(ArrayList_isEmpty(&ArrayList_G));
    std::vector<int> Array;
    AddLastDefault(Array);
    EXPECT_FALSE(ArrayList_isEmpty(&ArrayList_G));
    Array.clear();
}

// 测试Contains
TEST_F(ArrayListTest, Contains)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    EXPECT_TRUE(ArrayList_Contains(Int, &ArrayList_G, NUMBER / 2));
    Array.clear();
}

// 测试Clear
TEST_F(ArrayListTest, Clear)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    ArrayList_Clear(&ArrayList_G);
    EXPECT_EQ(ArrayList_GetSize(&ArrayList_G), 0);
    EXPECT_TRUE(ArrayList_isEmpty(&ArrayList_G));
    Array.clear();
}

// 测试Destroy
TEST_F(ArrayListTest, Destroy)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    ArrayList_Destroy(Int, &ArrayList_G);
    EXPECT_EQ(ArrayList_GetSize(&ArrayList_G), 0);
    EXPECT_TRUE(ArrayList_isEmpty(&ArrayList_G));
    EXPECT_EQ(ArrayList_G.Array, nullptr);
    Array.clear();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

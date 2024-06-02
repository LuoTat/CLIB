#include <gtest/gtest.h>
#include <vector>
#include "LinkedList_G.h"

// 定义比较函数
#define INTCMP(a, b) ((a) == (b))

// 定义元素个数
#define NUMBER       1000000

// 初始化LinkedList类型
LTT_LINKEDLIST_INIT(INT, int, INTCMP);

class LinkedListTest: public ::testing::Test
{
protected:
    LinkedList(INT) LinkedList_G;

    void SetUp() override
    {
        LinkedList_Init(INT, &LinkedList_G);
        EXPECT_EQ(LinkedList_GetSize(&LinkedList_G), 0);
        EXPECT_TRUE(LinkedList_IsEmpty(&LinkedList_G));
    }

    void TearDown() override
    {
        LinkedList_Destroy(INT, &LinkedList_G);
        EXPECT_EQ(LinkedList_GetSize(&LinkedList_G), 0);
        EXPECT_TRUE(LinkedList_IsEmpty(&LinkedList_G));
        EXPECT_EQ(LinkedList_G.EndNode, nullptr);
    }

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

    // 将Array的元素AddFirst到LinkedList_G
    void AddFirstFromArray(std::vector<int>& Array)
    {
        for (auto i : Array) { EXPECT_EQ(LinkedList_AddFirst(INT, &LinkedList_G, i), Success); }
    }

    // 将Array的元素AddLast到LinkedList_G
    void AddLastFromArray(std::vector<int>& Array)
    {
        for (auto i : Array) { EXPECT_EQ(LinkedList_AddLast(INT, &LinkedList_G, i), Success); }
    }

    // 从LinkedList_G DeleteFirst Number个元素到Array中
    void DeleteFirstToArray(int* Array, int Number)
    {
        for (int i = 0; i < Number; ++i) { EXPECT_EQ(LinkedList_DeleteFirst(INT, &LinkedList_G, &Array[i]), Success); }
    }

    // 从LinkedList_G DeleteLast Number个元素到Array中
    void DeleteLastToArray(int* Array, int Number)
    {
        for (int i = 0; i < Number; ++i) { EXPECT_EQ(LinkedList_DeleteLast(INT, &LinkedList_G, &Array[i]), Success); }
    }
};

// 测试AddFirst
TEST_F(LinkedListTest, AddFirst)
{
    std::vector<int> Array;
    AddFirstDefault(Array);
    // 逐个比较
    LinkedListNode_INT* Node = LinkedList_G.EndNode->Next;
    for (int i = 0; i < NUMBER; ++i)
    {
        EXPECT_EQ(Node->Data, Array[NUMBER - i - 1]);
        Node = Node->Next;
    }
    // Size是否正确
    EXPECT_EQ(LinkedList_GetSize(&LinkedList_G), NUMBER);
    Array.clear();
}

// 测试AddLast
TEST_F(LinkedListTest, AddLast)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    // 逐个比较
    LinkedListNode_INT* Node = LinkedList_G.EndNode->Next;
    for (int i = 0; i < NUMBER; ++i)
    {
        EXPECT_EQ(Node->Data, Array[i]);
        Node = Node->Next;
    }
    // Size是否正确
    EXPECT_EQ(LinkedList_GetSize(&LinkedList_G), NUMBER);
    Array.clear();
}

// 测试AddIndex
TEST_F(LinkedListTest, AddIndex)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    // 在Index处插入0
    int Index = NUMBER / 2;
    EXPECT_EQ(LinkedList_AddIndex(INT, &LinkedList_G, 0, Index), Success);
    // 逐个从0到Index-1比较
    LinkedListNode_INT* Node = LinkedList_G.EndNode->Next;
    for (int i = 0; i < Index; ++i)
    {
        EXPECT_EQ(Node->Data, Array[i]);
        Node = Node->Next;
    }
    // Index是否为0
    EXPECT_EQ(Node->Data, 0);
    Node = Node->Next;
    // 逐个从Index+1到NUMBER+1比较
    for (int i = Index + 1; i < NUMBER + 1; ++i)
    {
        EXPECT_EQ(Node->Data, Array[i - 1]);
        Node = Node->Next;
    }
    // Size是否正确
    EXPECT_EQ(LinkedList_GetSize(&LinkedList_G), NUMBER + 1);
    Array.clear();
}

// 测试DeleteFirst
TEST_F(LinkedListTest, DeleteFirst)
{
    std::vector<int> InArray;
    AddLastDefault(InArray);
    int* OutArray = new int[NUMBER];
    DeleteFirstDefault(OutArray);
    // 逐个比较
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(OutArray[i], InArray[i]); }
    // Size是否正确
    EXPECT_EQ(LinkedList_GetSize(&LinkedList_G), 0);
    // 是否为空
    EXPECT_TRUE(LinkedList_IsEmpty(&LinkedList_G));
    InArray.clear();
    delete[] OutArray;
}

// 测试DeleteLast
TEST_F(LinkedListTest, DeleteLast)
{
    std::vector<int> InArray;
    AddLastDefault(InArray);
    int* OutArray = new int[NUMBER];
    DeleteLastDefault(OutArray);
    // 逐个比较
    for (int i = 0; i < NUMBER; ++i) { EXPECT_EQ(OutArray[i], InArray[NUMBER - i - 1]); }
    // Size是否正确
    EXPECT_EQ(LinkedList_GetSize(&LinkedList_G), 0);
    // 是否为空
    EXPECT_TRUE(LinkedList_IsEmpty(&LinkedList_G));
    InArray.clear();
    delete[] OutArray;
}

// 测试DeleteIndex
TEST_F(LinkedListTest, DeleteIndex)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    int result;
    int Index = NUMBER / 2;
    // 删除Index处的元素
    EXPECT_EQ(LinkedList_DeleteIndex(INT, &LinkedList_G, Index, &result), Success);
    // Index处的返回的原元素是否正确
    EXPECT_EQ(result, Array[Index]);
    // 逐个比较从0到Index-1
    LinkedListNode_INT* Node = LinkedList_G.EndNode->Next;
    for (int i = 0; i < Index; ++i)
    {
        EXPECT_EQ(Node->Data, Array[i]);
        Node = Node->Next;
    }
    // 逐个比较从Index到NUMBER-1
    for (int i = Index + 1; i < NUMBER; ++i)
    {
        EXPECT_EQ(Node->Data, Array[i]);
        Node = Node->Next;
    }
    Array.clear();
}

// 测试SetIndex
TEST_F(LinkedListTest, SetIndex)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    int result;
    int Index = NUMBER / 2;
    EXPECT_EQ(LinkedList_SetIndex(INT, &LinkedList_G, 0, Index, &result), Success);
    // Index处的返回的原元素是否正确
    EXPECT_EQ(result, Array[Index]);
    LinkedListNode_INT* Node = LinkedList_G.EndNode->Next;
    for (int i = 0; i < Index; ++i)
    {
        EXPECT_EQ(Node->Data, Array[i]);
        Node = Node->Next;
    }
    EXPECT_EQ(Node->Data, 0);
    Node = Node->Next;
    for (int i = Index + 1; i < NUMBER; ++i)
    {
        EXPECT_EQ(Node->Data, Array[i]);
        Node = Node->Next;
    }
    Array.clear();
}

// 测试GetFirst
TEST_F(LinkedListTest, GetFirst)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    int result;
    EXPECT_EQ(LinkedList_GetFirst(INT, &LinkedList_G, &result), Success);
    EXPECT_EQ(result, Array[0]);
    Array.clear();
}

// 测试GetLast
TEST_F(LinkedListTest, GetLast)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    int result;
    EXPECT_EQ(LinkedList_GetLast(INT, &LinkedList_G, &result), Success);
    EXPECT_EQ(result, Array[NUMBER - 1]);
    Array.clear();
}

// 测试GetIndex
TEST_F(LinkedListTest, GetIndex)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    int result;
    int Index = NUMBER / 2;
    EXPECT_EQ(LinkedList_GetIndex(INT, &LinkedList_G, Index, &result), Success);
    EXPECT_EQ(result, Array[Index]);
    Array.clear();
}

// 测试IndexOf
TEST_F(LinkedListTest, IndexOf)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    int Temp;
    // 将首尾元素设置为0
    EXPECT_EQ(LinkedList_SetIndex(INT, &LinkedList_G, 0, NUMBER - 1, &Temp), Success);
    EXPECT_EQ(LinkedList_IndexOf(INT, &LinkedList_G, 0), 0);
    Array.clear();
}

// 测试LastIndexOf
TEST_F(LinkedListTest, LastIndexOf)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    int Temp;
    // 将首尾元素设置为0
    EXPECT_EQ(LinkedList_SetIndex(INT, &LinkedList_G, 0, NUMBER - 1, &Temp), Success);
    EXPECT_EQ(LinkedList_LastIndexOf(INT, &LinkedList_G, 0), NUMBER - 1);
    Array.clear();
}

// 测试GetSize
TEST_F(LinkedListTest, GetSize)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    EXPECT_EQ(LinkedList_GetSize(&LinkedList_G), NUMBER);
    Array.clear();
}

// 测试IsEmpty
TEST_F(LinkedListTest, IsEmpty)
{
    EXPECT_TRUE(LinkedList_IsEmpty(&LinkedList_G));
    std::vector<int> Array;
    AddLastDefault(Array);
    EXPECT_FALSE(LinkedList_IsEmpty(&LinkedList_G));
    Array.clear();
}

// 测试Contains
TEST_F(LinkedListTest, Contains)
{
    std::vector<int> Array;
    AddLastDefault(Array);
    EXPECT_TRUE(LinkedList_Contains(INT, &LinkedList_G, NUMBER / 2));
    Array.clear();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
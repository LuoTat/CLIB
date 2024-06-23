#include <gtest/gtest.h>
#include <unordered_set>
#include "HashSet_G.h"

typedef struct Student
{
    int ID;
    int age;
} Student;

// 定义元素个数
#define NUMBER 100000

// 初始化HashSet类型
LTT_HASHSET_INT_INIT(INT);

LTT_HASHSET_INIT_DEFAULT(STU, Student);

class HashSetTest: public ::testing::Test
{
protected:
    HashSet(INT) HashSet_int;
    HashSet(STU) HashSet_stu;

    void SetUp() override
    {
        HashSet_Init(INT, &HashSet_int);
        HashSet_Init(STU, &HashSet_stu);
        EXPECT_EQ(HashSet_GetSize(&HashSet_int), 0);
        EXPECT_TRUE(HashSet_IsEmpty(&HashSet_int));
        EXPECT_EQ(HashSet_GetSize(&HashSet_stu), 0);
        EXPECT_TRUE(HashSet_IsEmpty(&HashSet_stu));
    }

    void TearDown() override
    {
        HashSet_Destroy(INT, &HashSet_int);
        HashSet_Destroy(STU, &HashSet_stu);
        EXPECT_EQ(HashSet_GetSize(&HashSet_int), 0);
        EXPECT_TRUE(HashSet_IsEmpty(&HashSet_int));
        EXPECT_EQ(HashSet_GetSize(&HashSet_stu), 0);
        EXPECT_TRUE(HashSet_IsEmpty(&HashSet_stu));
    }

    void AddDefault_INT(HashSet(INT) & HashSet)
    {
        for (int i = 0; i < NUMBER; ++i) { EXPECT_TRUE(HashSet_Add(INT, &HashSet, i)); }
    }

    void AddDefault_STU(HashSet(STU) & HashSet)
    {
        Student stu;
        for (int i = 0; i < NUMBER; ++i)
        {
            stu.ID  = i;
            stu.age = i % 18;
            EXPECT_TRUE(HashSet_Add(STU, &HashSet, stu));
        }
    }
};

// 测试Add
TEST_F(HashSetTest, Add)
{
    AddDefault_INT(HashSet_int);
    EXPECT_EQ(HashSet_GetSize(&HashSet_int), NUMBER);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_FALSE(HashSet_Add(INT, &HashSet_int, i)); }

    AddDefault_STU(HashSet_stu);
    EXPECT_EQ(HashSet_GetSize(&HashSet_stu), NUMBER);
    for (int i = 0; i < NUMBER; ++i)
    {
        Student stu;
        stu.ID  = i;
        stu.age = i % 18;
        EXPECT_FALSE(HashSet_Add(STU, &HashSet_stu, stu));
    }
}

// 测试Delete
TEST_F(HashSetTest, Delete)
{
    AddDefault_INT(HashSet_int);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_TRUE(HashSet_Delete(INT, &HashSet_int, i)); }
    EXPECT_EQ(HashSet_GetSize(&HashSet_int), 0);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_FALSE(HashSet_Delete(INT, &HashSet_int, i)); }
    AddDefault_INT(HashSet_int);
    for (int i = NUMBER; i < 2 * NUMBER; ++i) { EXPECT_FALSE(HashSet_Delete(INT, &HashSet_int, i)); }

    AddDefault_STU(HashSet_stu);
    for (int i = 0; i < NUMBER; ++i)
    {
        Student stu;
        stu.ID  = i;
        stu.age = i % 18;
        EXPECT_TRUE(HashSet_Delete(STU, &HashSet_stu, stu));
    }
    EXPECT_EQ(HashSet_GetSize(&HashSet_stu), 0);
    for (int i = 0; i < NUMBER; ++i)
    {
        Student stu;
        stu.ID  = i;
        stu.age = i % 18;
        EXPECT_FALSE(HashSet_Delete(STU, &HashSet_stu, stu));
    }
    AddDefault_STU(HashSet_stu);
    for (int i = NUMBER; i < 2 * NUMBER; ++i)
    {
        Student stu;
        stu.ID  = i;
        stu.age = i % 18;
        EXPECT_FALSE(HashSet_Delete(STU, &HashSet_stu, stu));
    }
}

// 测试Contains
TEST_F(HashSetTest, Contains)
{
    AddDefault_INT(HashSet_int);
    for (int i = 0; i < NUMBER; ++i) { EXPECT_TRUE(HashSet_Contains(INT, &HashSet_int, i)); }
    for (int i = NUMBER; i < 2 * NUMBER; ++i) { EXPECT_FALSE(HashSet_Contains(INT, &HashSet_int, i)); }

    AddDefault_STU(HashSet_stu);
    for (int i = 0; i < NUMBER; ++i)
    {
        Student stu;
        stu.ID  = i;
        stu.age = i % 18;
        EXPECT_TRUE(HashSet_Contains(STU, &HashSet_stu, stu));
    }
    for (int i = NUMBER; i < 2 * NUMBER; ++i)
    {
        Student stu;
        stu.ID  = i;
        stu.age = i % 18;
        EXPECT_FALSE(HashSet_Contains(STU, &HashSet_stu, stu));
    }
}

// 测试IsEmpty
TEST_F(HashSetTest, IsEmpty)
{
    EXPECT_TRUE(HashSet_IsEmpty(&HashSet_int));
    EXPECT_TRUE(HashSet_IsEmpty(&HashSet_stu));
    AddDefault_INT(HashSet_int);
    EXPECT_FALSE(HashSet_IsEmpty(&HashSet_int));
    AddDefault_STU(HashSet_stu);
    EXPECT_FALSE(HashSet_IsEmpty(&HashSet_stu));
}

// 测试GetSize
TEST_F(HashSetTest, GetSize)
{
    EXPECT_EQ(HashSet_GetSize(&HashSet_int), 0);
    EXPECT_EQ(HashSet_GetSize(&HashSet_stu), 0);
    AddDefault_INT(HashSet_int);
    EXPECT_EQ(HashSet_GetSize(&HashSet_int), NUMBER);
    AddDefault_STU(HashSet_stu);
    EXPECT_EQ(HashSet_GetSize(&HashSet_stu), NUMBER);
}

// 测试Clear
TEST_F(HashSetTest, Clear)
{
    AddDefault_INT(HashSet_int);
    HashSet_Clear(INT, &HashSet_int);
    EXPECT_EQ(HashSet_GetSize(&HashSet_int), 0);
    EXPECT_TRUE(HashSet_IsEmpty(&HashSet_int));

    AddDefault_STU(HashSet_stu);
    HashSet_Clear(STU, &HashSet_stu);
    EXPECT_EQ(HashSet_GetSize(&HashSet_stu), 0);
    EXPECT_TRUE(HashSet_IsEmpty(&HashSet_stu));
}
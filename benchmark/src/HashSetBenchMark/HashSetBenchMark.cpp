#include <benchmark/benchmark.h>
#include <unordered_set>
#include "HashSet_G.h"

typedef struct Student
{
    int ID;
    int age;
} Student;

struct StudentHash
{
    size_t operator()(const Student& stu) const { return stu.ID + stu.age; }
};

struct StudentEqual
{
    bool operator()(const Student& stu1, const Student& stu2) const { return stu1.ID == stu2.ID && stu1.age == stu2.age; }
};

#define Student_Hash_Function(Key)          ((Key).ID + (Key).age)
#define Student_Equals_Function(Key1, Key2) ((Key1).ID == (Key2).ID && (Key1).age == (Key2).age)

// 初始化HashSet类型
LTT_HASHSET_INT_INIT(INT);

LTT_HASHSET_INIT(STU, Student, Student_Hash_Function, Student_Equals_Function);

#define NUMBERS       100000

// 测试HashSet_int Add性能
static void HashSet_int_Add_Test(benchmark::State& state)
{
    HashSet(INT) HashSet_int;
    for (auto _ : state)
    {
        HashSet_Init(INT, &HashSet_int);
        for (int i = 0; i < state.range(0); ++i) HashSet_Add(INT, &HashSet_int, i);
        HashSet_Destroy(INT, &HashSet_int);
    }
}

// 测试HashSet_int Delete性能
static void HashSet_int_Delete_Test(benchmark::State& state)
{
    HashSet(INT) HashSet_int;
    for (auto _ : state)
    {
        HashSet_Init(INT, &HashSet_int);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) HashSet_Add(INT, &HashSet_int, i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) HashSet_Delete(INT, &HashSet_int, i);
        HashSet_Destroy(INT, &HashSet_int);
    }
}

// 测试HashSet_int Contains性能
static void HashSet_int_Contains_Test(benchmark::State& state)
{
    HashSet(INT) HashSet_int;
    for (auto _ : state)
    {
        HashSet_Init(INT, &HashSet_int);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) HashSet_Add(INT, &HashSet_int, i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) HashSet_Contains(INT, &HashSet_int, i);
        HashSet_Destroy(INT, &HashSet_int);
    }
}

// 测试HashSet_stu Add性能
static void HashSet_stu_Add_Test(benchmark::State& state)
{
    HashSet(STU) HashSet_stu;
    for (auto _ : state)
    {
        HashSet_Init(STU, &HashSet_stu);
        for (int i = 0; i < state.range(0); ++i)
        {
            Student stu;
            stu.ID  = i;
            stu.age = 1 % 18;
            HashSet_Add(STU, &HashSet_stu, stu);
        }
        HashSet_Destroy(STU, &HashSet_stu);
    }
}

// 测试HashSet_stu Delete性能
static void HashSet_stu_Delete_Test(benchmark::State& state)
{
    HashSet(STU) HashSet_stu;
    for (auto _ : state)
    {
        HashSet_Init(STU, &HashSet_stu);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i)
        {
            Student stu;
            stu.ID  = i;
            stu.age = 1 % 18;
            HashSet_Add(STU, &HashSet_stu, stu);
        }
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i)
        {
            Student stu;
            stu.ID  = i;
            stu.age = 1 % 18;
            HashSet_Delete(STU, &HashSet_stu, stu);
        }
        HashSet_Destroy(STU, &HashSet_stu);
    }
}

// 测试HashSet_stu Contains性能
static void HashSet_stu_Contains_Test(benchmark::State& state)
{
    HashSet(STU) HashSet_stu;
    for (auto _ : state)
    {
        HashSet_Init(STU, &HashSet_stu);
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i)
        {
            Student stu;
            stu.ID  = i;
            stu.age = 1 % 18;
            HashSet_Add(STU, &HashSet_stu, stu);
        }
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i)
        {
            Student stu;
            stu.ID  = i;
            stu.age = 1 % 18;
            HashSet_Contains(STU, &HashSet_stu, stu);
        }
        HashSet_Destroy(STU, &HashSet_stu);
    }
}

// 测试unordered_set_int Add性能
static void unordered_set_int_Add_Test(benchmark::State& state)
{
    std::unordered_set<int> unordered_set_int;
    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i) unordered_set_int.insert(i);
        unordered_set_int.clear();
    }
}

// 测试unordered_set_int Delete性能
static void unordered_set_int_Delete_Test(benchmark::State& state)
{
    std::unordered_set<int> unordered_set_int;
    for (auto _ : state)
    {
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) unordered_set_int.insert(i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) unordered_set_int.erase(i);
        unordered_set_int.clear();
    }
}

// 测试unordered_set_int Contains性能
static void unordered_set_int_Contains_Test(benchmark::State& state)
{
    std::unordered_set<int> unordered_set_int;
    for (auto _ : state)
    {
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i) unordered_set_int.insert(i);
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i) unordered_set_int.find(i);
        unordered_set_int.clear();
    }
}

// 测试unordered_set_stu Add性能
static void unordered_set_stu_Add_Test(benchmark::State& state)
{
    std::unordered_set<Student, StudentHash, StudentEqual> unordered_set_stu;
    for (auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i)
        {
            Student stu;
            stu.ID  = i;
            stu.age = 1 % 18;
            unordered_set_stu.insert(stu);
        }
        unordered_set_stu.clear();
    }
}

// 测试unordered_set_int Delete性能
static void unordered_set_stu_Delete_Test(benchmark::State& state)
{
    std::unordered_set<Student, StudentHash, StudentEqual> unordered_set_stu;
    for (auto _ : state)
    {
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i)
        {
            Student stu;
            stu.ID  = i;
            stu.age = 1 % 18;
            unordered_set_stu.insert(stu);
        }
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i)
        {
            Student stu;
            stu.ID  = i;
            stu.age = 1 % 18;
            unordered_set_stu.erase(stu);
        }
        unordered_set_stu.clear();
    }
}

// 测试unordered_set_int Contains性能
static void unordered_set_stu_Contains_Test(benchmark::State& state)
{
    std::unordered_set<Student, StudentHash, StudentEqual> unordered_set_stu;
    for (auto _ : state)
    {
        state.PauseTiming();
        for (int i = 0; i < state.range(0); ++i)
        {
            Student stu;
            stu.ID  = i;
            stu.age = 1 % 18;
            unordered_set_stu.insert(stu);
        }
        state.ResumeTiming();
        for (int i = 0; i < state.range(0); ++i)
        {
            Student stu;
            stu.ID  = i;
            stu.age = 1 % 18;
            unordered_set_stu.find(stu);
        }
        unordered_set_stu.clear();
    }
}

BENCHMARK(HashSet_int_Add_Test)->Arg(NUMBERS);
BENCHMARK(HashSet_int_Delete_Test)->Arg(NUMBERS);
BENCHMARK(HashSet_int_Contains_Test)->Arg(NUMBERS);
BENCHMARK(HashSet_stu_Add_Test)->Arg(NUMBERS);
BENCHMARK(HashSet_stu_Delete_Test)->Arg(NUMBERS);
BENCHMARK(HashSet_stu_Contains_Test)->Arg(NUMBERS);
BENCHMARK(unordered_set_int_Add_Test)->Arg(NUMBERS);
BENCHMARK(unordered_set_int_Delete_Test)->Arg(NUMBERS);
BENCHMARK(unordered_set_int_Contains_Test)->Arg(NUMBERS);
BENCHMARK(unordered_set_stu_Add_Test)->Arg(NUMBERS);
BENCHMARK(unordered_set_stu_Delete_Test)->Arg(NUMBERS);
BENCHMARK(unordered_set_stu_Contains_Test)->Arg(NUMBERS);
BENCHMARK_MAIN();
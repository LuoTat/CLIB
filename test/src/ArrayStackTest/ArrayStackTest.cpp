#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stack>
#include "ArrayStack.h"
#include "ArrayStack_G.h"

#define StudentCmp(a, b) ((a).age == (b).age && (a).score == (b).score)
#define INTCmp(a, b)     ((a) == (b))

int cmp(int a, int b) { return a == b; }


LTT_ARRAYSTACK_INIT(INT, int, cmp)

using namespace std;

#define NUMBER 100000000

// typedef struct Student
// {
//     int age;
//     int score;
// } Student;

// LTT_ARRAYSTACK_INIT(STUDENT, Student,StudentCmp)

int main()
{
    struct timespec start, end;
    ArrayStack*     ArrayStack    = LTT_ArrayStack_New(sizeof(int), NULL);
    ArrayStack(INT)* ArrayStack_G = ArrayStack_New(INT);
    stack<int> stack;
    int*       array = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { LTT_ArrayStack_Push(ArrayStack, &array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("ArrayStack      插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    LTT_ArrayStack_Destroy(&ArrayStack);

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { ArrayStack_Push(INT, ArrayStack_G, array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("ArrayStack_G    插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    ArrayStack_Destroy(INT, ArrayStack_G);

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { stack.push(array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("stack           插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);

    // Student stu1                      = {18, 100};
    // Student stu2                      = {19, 99};
    // Student stu3                      = {20, 98};
    // ArrayStack(STUDENT)* ArrayStack_G = ArrayStack_New(STUDENT);
    // ArrayStack_Push(STUDENT, ArrayStack_G, stu1);
    // ArrayStack_Push(STUDENT, ArrayStack_G, stu2);
    // ArrayStack_Push(STUDENT, ArrayStack_G, stu3);

    // Student stu4 = {18, 101};

    // printf(ArrayStack_Contains(STUDENT, ArrayStack_G, stu4) ? "true\n" : "false\n");

    printf("Test Over!\n");
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stack>
#include "ArrayStack_G.h"

#define INTCMP(a, b) ((a) == (b))
LTT_ARRAYSTACK_INIT(INT, int, INTCMP)

using namespace std;

#define NUMBER 100000000

int main()
{
    timespec start, end;
    ArrayStack(INT) ArrayStack_G;
    ArrayStack_Init(INT, &ArrayStack_G);
    stack<int> stack;
    int*       array = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { ArrayStack_Push(INT, &ArrayStack_G, array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("ArrayStack_G  入栈%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    int Temp;
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { ArrayStack_Pop(INT, &ArrayStack_G, &Temp); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("ArrayStack_G  出栈%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    ArrayStack_Destroy(INT, &ArrayStack_G);

    printf("#######################################################################\n");

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { stack.push(array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("stack         入栈%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { stack.pop(); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("stack         出栈%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);


    printf("Test Over!\n");
    return 0;
}
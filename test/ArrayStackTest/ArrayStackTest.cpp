#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stack>
#include "LTT_ArrayStack.h"


using namespace std;

#define NUMBER 100000000

void PRINT(ArrayStack* ArrayStack)
{
    Iterator iterator = LTT_ArrayStack_GetIterator(ArrayStack);
    while (iterator.MoveNext(&iterator)) { printf("%d ", *(int*)LTT_ArrayStack_GetCurrent(&iterator)); }
    printf("\n");
}

int main()
{
    clock_t     start, end;
    ArrayStack* ArrayStack = LTT_ArrayStack_New(sizeof(int), NULL);
    stack<int>  stack;
    int*        array = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }

    start = clock();
    for (int i = 0; i < NUMBER; i++) { LTT_ArrayStack_Push(ArrayStack, &array[i]); }
    end = clock();
    printf("LTT_ArrayStack 插入%d个元素耗时: %.4lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);

    start = clock();
    for (int i = 0; i < NUMBER; i++) { stack.push(array[i]); }
    end = clock();
    printf("stack          插入%d个元素耗时: %.4lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);

    printf("Test Over!\n");
    return 0;
}
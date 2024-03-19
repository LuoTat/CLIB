#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <deque>
#include "LTT_ArrayDeque.h"

using namespace std;

#define NUMBER 100000000

void PRINT(ArrayDeque* ArrayDeque)
{
    Iterator iterator = LTT_ArrayDeque_GetIterator(ArrayDeque);
    while (iterator.MoveNext(&iterator)) { printf("%d ", *(int*)LTT_ArrayDeque_GetCurrent(&iterator)); }
    printf("\n");
}

int main()
{
    clock_t     start, end;
    ArrayDeque* ArrayDeque = LTT_ArrayDeque_New(sizeof(int), NULL);
    deque<int>  deque;
    int*        array = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }

    start = clock();
    for (int i = 0; i < NUMBER; i++) { LTT_ArrayDeque_AddFirst(ArrayDeque, &array[i]); }
    end = clock();
    printf("LTT_ArrayDeque 头部插入%d个元素耗时: %.4lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);

    start = clock();
    for (int i = 0; i < NUMBER; i++) { deque.push_back(array[i]); }
    end = clock();
    printf("deque          头部插入%d个元素耗时: %.4lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);

    printf("Test Over!\n");
    return 0;
}
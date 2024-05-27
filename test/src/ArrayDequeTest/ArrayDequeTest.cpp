#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <deque>
#include "ArrayDeque.h"

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
    timespec    start, end;
    ArrayDeque* ArrayDeque = LTT_ArrayDeque_New(sizeof(int), NULL);
    deque<int>  deque;
    int*        array = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { LTT_ArrayDeque_AddFirst(ArrayDeque, &array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LTT_ArrayDeque 头部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { deque.push_back(array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("deque          头部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);

    printf("Test Over!\n");
    return 0;
}
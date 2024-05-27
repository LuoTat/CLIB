#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue>
#include "ArrayQueue.h"

using namespace std;

#define NUMBER 100000000

void PRINT(ArrayQueue* ArrayQueue)
{
    Iterator iterator = LTT_ArrayQueue_GetIterator(ArrayQueue);
    while (iterator.MoveNext(&iterator)) { printf("%d ", *(int*)LTT_ArrayQueue_GetCurrent(&iterator)); }
    printf("\n");
}

int main()
{
    timespec    start, end;
    ArrayQueue* ArrayQueue = LTT_ArrayQueue_New(sizeof(int), NULL);
    queue<int>  queue;
    int*        array = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { LTT_ArrayQueue_Push(ArrayQueue, &array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LTT_ArrayQueue 插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { queue.push(array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("queue          插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);

    printf("Test Over!\n");
    return 0;
}
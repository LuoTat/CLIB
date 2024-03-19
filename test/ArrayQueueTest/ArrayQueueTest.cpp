#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue>
#include "LTT_ArrayQueue.h"

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
    clock_t     start, end;
    ArrayQueue* ArrayQueue = LTT_ArrayQueue_New(sizeof(int), NULL);
    queue<int>  queue;
    int*        array = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }

    start = clock();
    for (int i = 0; i < NUMBER; i++) { LTT_ArrayQueue_Push(ArrayQueue, &array[i]); }
    end = clock();
    printf("LTT_ArrayQueue 插入%d个元素耗时: %.4lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);

    start = clock();
    for (int i = 0; i < NUMBER; i++) { queue.push(array[i]); }
    end = clock();
    printf("queue          插入%d个元素耗时: %.4lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);

    printf("Test Over!\n");
    return 0;
}
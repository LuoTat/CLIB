#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <deque>
#include "ArrayDeque.h"
#include "ArrayDeque_G.h"

#define INTCMP(a, b) ((a) == (b))
LTT_ARRAYDEQUE_INIT(INT, int, INTCMP)

using namespace std;

#define NUMBER 100000000

void PRINT(ArrayDeque* ArrayDeque)
{
    Iterator iterator = LTT_ArrayDeque_GetIterator(ArrayDeque);
    while (iterator.MoveNext(&iterator)) { printf("%d ", *(int*)LTT_ArrayDeque_GetCurrent(&iterator)); }
    printf("\n");
}

void printfG(ArrayDeque(INT) * ArrayDeque)
{
    for (int i = 0; i < ArrayDeque->Capacity; i++) { printf("%2d ", ArrayDeque->Array[i]); }
    printf("\n");
}

int main()
{
    // ArrayDeque(INT) ArrayDeque_G;
    // ArrayDeque_Init(INT, &ArrayDeque_G);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 1);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 2);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 3);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 4);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 5);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 6);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 7);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 8);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 9);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 10);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 11);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 12);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 13);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 14);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 15);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 16);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 17);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 18);
    // printfG(&ArrayDeque_G);
    // ArrayDeque_AddLast(INT, &ArrayDeque_G, 19);
    // printfG(&ArrayDeque_G);
    // printf("ArrayDeque_G.Size: %d\n", ArrayDeque_GetSize(&ArrayDeque_G));
    // int Temp = 0;
    // ArrayDeque_DeleteFirst(INT, &ArrayDeque_G, &Temp);
    // printf("ArrayDeque_G.Size: %d\n", ArrayDeque_GetSize(&ArrayDeque_G));
    // ArrayDeque_DeleteFirst(INT, &ArrayDeque_G, &Temp);
    // printf("ArrayDeque_G.Size: %d\n", ArrayDeque_GetSize(&ArrayDeque_G));
    // ArrayDeque_DeleteFirst(INT, &ArrayDeque_G, &Temp);
    // printf("ArrayDeque_G.Size: %d\n", ArrayDeque_GetSize(&ArrayDeque_G));
    // ArrayDeque_DeleteFirst(INT, &ArrayDeque_G, &Temp);
    // printf("ArrayDeque_G.Size: %d\n", ArrayDeque_GetSize(&ArrayDeque_G));
    // ArrayDeque_DeleteFirst(INT, &ArrayDeque_G, &Temp);
    // printf("ArrayDeque_G.Size: %d\n", ArrayDeque_GetSize(&ArrayDeque_G));
    // printf("First: %d\n", Temp);

    // ArrayDeque_DeleteLast(INT, &ArrayDeque_G, &Temp);
    // printf("ArrayDeque_G.Size: %d\n", ArrayDeque_GetSize(&ArrayDeque_G));
    // ArrayDeque_DeleteLast(INT, &ArrayDeque_G, &Temp);
    // printf("ArrayDeque_G.Size: %d\n", ArrayDeque_GetSize(&ArrayDeque_G));
    // ArrayDeque_DeleteLast(INT, &ArrayDeque_G, &Temp);
    // printf("ArrayDeque_G.Size: %d\n", ArrayDeque_GetSize(&ArrayDeque_G));
    // ArrayDeque_DeleteLast(INT, &ArrayDeque_G, &Temp);
    // printf("ArrayDeque_G.Size: %d\n", ArrayDeque_GetSize(&ArrayDeque_G));
    // ArrayDeque_DeleteLast(INT, &ArrayDeque_G, &Temp);
    // printf("ArrayDeque_G.Size: %d\n", ArrayDeque_GetSize(&ArrayDeque_G));
    // printf("Last: %d\n", Temp);




    timespec    start, end;
    ArrayDeque* ArrayDeque = LTT_ArrayDeque_New(sizeof(int), NULL);
    ArrayDeque(INT) ArrayDeque_G;
    ArrayDeque_Init(INT, &ArrayDeque_G);
    deque<int> deque;
    int*       array = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }


    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { LTT_ArrayDeque_AddFirst(ArrayDeque, &array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LTT_ArrayDeque  头部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { LTT_ArrayDeque_AddLast(ArrayDeque, &array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LTT_ArrayDeque  尾部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { LTT_ArrayDeque_DeleteFirst(ArrayDeque); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LTT_ArrayDeque  头部删除%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { LTT_ArrayDeque_DeleteLast(ArrayDeque); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LTT_ArrayDeque  尾部删除%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    LTT_ArrayDeque_Destroy(&ArrayDeque);

    printf("#######################################################################\n");

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { ArrayDeque_AddFirst(INT, &ArrayDeque_G, array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("ArrayDeque_G    头部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { ArrayDeque_AddLast(INT, &ArrayDeque_G, array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("ArrayDeque_G    尾部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    int Temp;
    for (int i = 0; i < NUMBER; i++) { ArrayDeque_DeleteFirst(INT, &ArrayDeque_G, &Temp); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("ArrayDeque_G    头部删除%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { ArrayDeque_DeleteLast(INT, &ArrayDeque_G, &Temp); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("ArrayDeque_G    尾部删除%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    ArrayDeque_Destroy(INT, &ArrayDeque_G);

    printf("#######################################################################\n");

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { deque.push_back(array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("deque           头部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { deque.push_front(array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("deque           尾部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { deque.pop_back(); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("deque           头部删除%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { deque.pop_front(); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("deque           尾部删除%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    deque.clear();

    printf("Test Over!\n");
    return 0;
}
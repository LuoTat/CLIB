#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include "LinkedList.h"
#include "LinkedList_G.h"

#define INTCMP(a, b) ((a) == (b))

LTT_LINKEDLIST_INIT(INT, int, INTCMP)

using namespace std;

#define NUMBER 100000000

void PRINT(LinkedList* LinkedList)
{
    Iterator iterator = LTT_LinkedList_GetIterator(LinkedList);
    while (iterator.MoveNext(&iterator)) { printf("%d ", *(int*)LTT_LinkedList_GetCurrent(&iterator)); }
    printf("\n");
}

int main()
{
    timespec    start, end;
    LinkedList* LinkedList = LTT_LinkedList_New(sizeof(int), NULL);
    LinkedList(INT) LinkedList_G;
    LinkedList_Init(INT, &LinkedList_G);
    list<int> list;
    int*      array = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { LTT_LinkedList_AddFirst(LinkedList, &array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LTT_LinkedList  头部插入%d个元素耗时: %11lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);

    for (int i = 0; i < NUMBER; i++) { LTT_LinkedList_AddLast(LinkedList, &array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LTT_LinkedList  尾部插入%d个元素耗时: %11lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);

    for (int i = 0; i < NUMBER; i++) { LTT_LinkedList_DeleteFirst(LinkedList); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LTT_LinkedList  头部删除%d个元素耗时: %11lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);

    for (int i = 0; i < NUMBER; i++) { LTT_LinkedList_DeleteLast(LinkedList); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LTT_LinkedList  尾部删除%d个元素耗时: %11lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);

    printf("#######################################################################\n");

    clock_gettime(CLOCK_REALTIME, &start);

    for (int i = 0; i < NUMBER; i++) { LinkedList_AddFirst(INT, &LinkedList_G, array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LinkedList_G    头部插入%d个元素耗时: %11lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);

    for (int i = 0; i < NUMBER; i++) { LinkedList_AddLast(INT, &LinkedList_G, array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LinkedList_G    尾部插入%d个元素耗时: %11lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);

    int Temp;
    for (int i = 0; i < NUMBER; i++) { LinkedList_DeleteFirst(INT, &LinkedList_G, &Temp); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LinkedList_G    头部删除%d个元素耗时: %11lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);

    for (int i = 0; i < NUMBER; i++) { LinkedList_DeleteLast(INT, &LinkedList_G, &Temp); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LinkedList_G    尾部删除%d个元素耗时: %11lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);

    printf("#######################################################################\n");

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { list.push_front(array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("list            头部插入%d个元素耗时: %11lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { list.push_back(array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("list            尾部插入%d个元素耗时: %11lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { list.pop_front(); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("list            头部删除%d个元素耗时: %11lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { list.pop_back(); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("list            尾部删除%d个元素耗时: %11lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);

    printf("Test Over!\n");
    return 0;
}
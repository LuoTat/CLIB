#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include "LTT_LinkedList.h"

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
    clock_t     start, end;
    LinkedList* LinkedList = LTT_LinkedList_New(sizeof(int), NULL);
    list<int>   list;
    int*        array = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }

    start = clock();
    for (int i = 0; i < NUMBER; i++) { LTT_LinkedList_AddFirst(LinkedList, &array[i]); }
    end = clock();
    printf("LTT_LinkedList 头部插入%d个元素耗时: %11.4lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC);
    start = clock();
    for (int i = 0; i < NUMBER; i++) { LTT_LinkedList_DeleteFirst(LinkedList); }
    end = clock();
    printf("LTT_LinkedList 头部删除%d个元素耗时: %11.4lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);
    start = clock();
    for (int i = 0; i < NUMBER; i++) { LTT_LinkedList_AddLast(LinkedList, &array[i]); }
    end = clock();
    printf("LTT_LinkedList 尾部插入%d个元素耗时: %11.4lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);
    start = clock();
    for (int i = 0; i < NUMBER; i++) { LTT_LinkedList_DeleteLast(LinkedList); }
    end = clock();
    printf("LTT_LinkedList 尾部删除%d个元素耗时: %11.4lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);

    printf("#######################################################################\n");

    start = clock();
    for (int i = 0; i < NUMBER; i++) { list.push_front(array[i]); }
    end = clock();
    printf("list           头部插入%d个元素耗时: %11lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);
    start = clock();
    for (int i = 0; i < NUMBER; i++) { list.pop_front(); }
    end = clock();
    printf("list           头部删除%d个元素耗时: %11lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);
    start = clock();
    for (int i = 0; i < NUMBER; i++) { list.push_back(array[i]); }
    end = clock();
    printf("list           尾部插入%d个元素耗时: %11lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);
    start = clock();
    for (int i = 0; i < NUMBER; i++) { list.pop_back(); }
    end = clock();
    printf("list           尾部删除%d个元素耗时: %11lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);

    printf("Test Over!\n");
    return 0;
}
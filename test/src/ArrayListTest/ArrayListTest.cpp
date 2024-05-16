#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "ArrayList.h"

using namespace std;

#define NUMBER 100000000

void PRINT(ArrayList* ArrayList)
{
    Iterator iterator = LTT_ArrayList_GetIterator(ArrayList);
    while (iterator.MoveNext(&iterator)) { printf("%d ", *(int*)LTT_ArrayList_GetCurrent(&iterator)); }
    printf("\n");
}

int main()
{
    clock_t     start, end;
    ArrayList*  ArrayList = LTT_ArrayList_New(sizeof(int), NULL);
    vector<int> vector;
    int*        array = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }

    start = clock();
    for (int i = 0; i < NUMBER; i++) { LTT_ArrayList_AddLast(ArrayList, &array[i]); }
    end = clock();
    printf("LTT_ArrayList 尾部插入%d个元素耗时: %.4lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);

    start = clock();
    for (int i = 0; i < NUMBER; i++) { vector.push_back(array[i]); }
    end = clock();
    printf("vector        尾部插入%d个元素耗时: %.4lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);

    printf("Test Over!\n");
    return 0;
}
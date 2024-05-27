#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "ArrayList.h"
#include "ArrayList_G.h"

#define INTCMP(a, b) ((a) == (b))

LTT_ARRAYLIST_INIT(INT, int, INTCMP)

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
    timespec   start, end;
    ArrayList* ArrayList = LTT_ArrayList_New(sizeof(int), NULL);
    ArrayList(INT) ArrayList_G;
    ArrayList_Init(INT, &ArrayList_G);
    vector<int> vector;
    int*        array = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { LTT_ArrayList_AddLast(ArrayList, &array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("LTT_ArrayList  尾部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    LTT_ArrayList_Destroy(&ArrayList);

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { ArrayList_AddLast(INT, &ArrayList_G, array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("ArrayList_G    尾部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    ArrayList_Destroy(INT, &ArrayList_G);

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { vector.push_back(array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("vector         尾部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    vector.clear();

    printf("Test Over!\n");
    return 0;
}
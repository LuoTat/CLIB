#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "ArrayList_G.h"

#define INTCMP(a, b) ((a) == (b))

LTT_ARRAYLIST_INIT(INT, int, INTCMP)

using namespace std;

#define NUMBER 100000

int main()
{
    timespec start, end;
    ArrayList(INT) ArrayList_G;
    ArrayList_Init(INT, &ArrayList_G);
    vector<int> vector;
    int*        array = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { ArrayList_AddFirst(INT, &ArrayList_G, array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("ArrayList_G  头部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { ArrayList_AddLast(INT, &ArrayList_G, array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("ArrayList_G  尾部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    int Temp;
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { ArrayList_DeleteFirst(INT, &ArrayList_G, &Temp); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("ArrayList_G  头部删除%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { ArrayList_DeleteLast(INT, &ArrayList_G, &Temp); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("ArrayList_G  尾部删除%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    ArrayList_Destroy(INT, &ArrayList_G);

    printf("#######################################################################\n");

    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { vector.insert(vector.begin(), array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("vector       头部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { vector.push_back(array[i]); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("vector       尾部插入%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { vector.erase(vector.begin()); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("vector       头部删除%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < NUMBER; i++) { vector.pop_back(); }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("vector       尾部删除%d个元素耗时: %lf ms\n", NUMBER, (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000.0);
    vector.clear();

    printf("Test Over!\n");
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unordered_map>
#include "HashMap.h"

using namespace std;

#define NUMBER 1000000

void PRINT(HashMap* HashMap)
{
    Iterator iterator = LTT_HashMap_GetIterator(HashMap);
    while (iterator.MoveNext(&iterator)) { printf("%d ", *(int*)LTT_HashMap_GetCurrent_Value(&iterator)); }
    printf("\n");
}

int main()
{
    clock_t  start, end;
    HashMap* HashMap                 = NULL;
    HashMap                          = LTT_HashMap_New(sizeof(int), sizeof(int), NULL, NULL, NULL);
    unordered_map<int, int> HashMap2 = unordered_map<int, int>();
    int*                    array    = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }

    start = clock();
    for (int i = 0; i < NUMBER; i++) { LTT_HashMap_Put(HashMap, &array[i], &array[i]); }
    end = clock();
    printf("LTT_HashMap   插入%d个键值对耗时: %12.4lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);

    start = clock();
    for (int i = 0; i < NUMBER; i++) { HashMap2[i] = i; }
    end = clock();
    printf("unordered_map 插入%d个键值对耗时: %12.4lf ms\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC * 1000);

    printf("Test Over!\n");
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LTT_UnionFindSet.h"

#define NUMBER 100000000

void PRINT(UnionFindSet* UnionFindSet)
{
    Iterator iterator = LTT_UnionFindSet_GetIterator(UnionFindSet);
    while (iterator.MoveNext(&iterator)) { printf("%d ", *(int*)LTT_UnionFindSet_GetCurrent(&iterator)); }
    printf("\n");
}

int main()
{
    clock_t       start, end;
    UnionFindSet* UnionFindSet = LTT_UnionFindSet_New(sizeof(int), NULL);
    int*          array        = (int*)malloc(NUMBER * sizeof(int));
    for (int i = 0; i < NUMBER; i++) { array[i] = i; }
    start = clock();
    for (int i = 0; i < NUMBER; i++) { LTT_UnionFindSet_Put(UnionFindSet, &array[i]); }
    end = clock();
    printf("插入%d个元素对耗时:%f\n", NUMBER, (double)(end - start) / CLOCKS_PER_SEC);
    start = clock();
    for (int i = 0; i < NUMBER / 2; i++) { LTT_UnionFindSet_Unite(UnionFindSet, &array[i], &array[i + 1]); }
    end = clock();
    printf("合并%d个元素对耗时:%f\n", NUMBER / 2, (double)(end - start) / CLOCKS_PER_SEC);
    printf("Test Over!\n");
    return 0;
}
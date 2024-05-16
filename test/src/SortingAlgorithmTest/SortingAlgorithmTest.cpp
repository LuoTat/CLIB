#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include "SortingAlgorithm.h"


using namespace std;
#define MAX_STRINGSLENGTH                        (50)
#define MAX_DOUBLELENGTH                         (12)


/*
    Mode == 1 //顺序数组
    Mode == 2 //逆序数组
    Mode == 3 //普通随机数组
    Mode == 4 //无相同元素的乱序数组
    Mode == 5 //有很多相同元素的数组
    Mode == 6 //16个元素的小数组
*/
#define NodeNum                                  50000000
#define Mode                                     3
#define PRINT                                    0
#define SortCounter                              10
#define RandomSend                               (unsigned)time(NULL)

#define InsertionSort_Test                       0
#define BinaryInsertionSort_Test                 0
#define ShellInsertionSort_Hibbard_Test          0
#define ShellInsertionSort_Sedgewick_Test        0
#define BubbleSort_Test                          0
#define BubbleSort_Fast_Test                     0
#define QuickSort_LTT_glibc_Test                 1
#define QuickSort_LTT_libstdcpp_Test             1
#define QuickSort_glibc_Test                     1
#define SimpleSelectionSort_Test                 0
#define HeapSort_Test                            0
#define MergeSort_Recursion_Test                 0
#define MergeSort_Iterative_Test                 0
#define MergeSort_Inplace_Iterative_Test         0
#define MergeSort_Inplace_Iterative_For_Int_Test 0
#define RadixSort_LSD_Test                       0
#define RadixSort_MSD_Test                       0
#define BucketSort_Test                          0
#define CountingSort_Test                        0
#define PigeonholeSort_Test                      0
#define BigoSort_Test                            0
#define IntrospectiveSort_Test                   1
#define sort_Test                                1
#define qsort_Test                               1

void SWAP(int* a, int* b)
{
    int temp = *a;
    *a       = *b;
    *b       = temp;
}

void GetTheRanddomNonRepeatingArray(int* Array, int NumOfElements)
{
    for (int i = 0; i < NumOfElements; ++i) Array[i] = i;
    for (int i = 0; i <= NodeNum - 2; ++i) { SWAP(&Array[i], &Array[i + (rand() % (NodeNum - i))]); }
}

void IsOrdered(int* a, int* b, int NumOfElements)
{
    for (int i = 0; i < NumOfElements; ++i)
    {
        if (a[i] != b[i])
        {
            printf("排序失误!!!!\n");
            exit(-1);
        }
    }
}

long long int CMPNUM = 0;

inline int getValue(const void* a) { return *(int*)a; }

inline int cmp(const void* a, const void* b)
{
    ++CMPNUM;
    return *(int*)a - *(int*)b;
}

inline bool cmp2(int a, int b)
{
    ++CMPNUM;
    return a < b;
}

int main()
{
    srand(RandomSend);
    int* Temp = (int*)malloc(NodeNum * sizeof(int));


#if InsertionSort_Test == 1
    double InsertionSort_SortSUM = 0;
#endif
#if BinaryInsertionSort_Test == 1
    double BinaryInsertionSort_SortSUM = 0;
#endif
#if ShellInsertionSort_Hibbard_Test == 1
    double ShellInsertionSort_Hibbard_SortSUM = 0;
#endif
#if ShellInsertionSort_Sedgewick_Test == 1
    double ShellInsertionSort_Sedgewick_SortSUM = 0;
#endif
#if BubbleSort_Test == 1
    double BubbleSort_SortSUM = 0;
#endif
#if BubbleSort_Fast_Test == 1
    double BubbleSort_Fast_SortSUM = 0;
#endif
#if QuickSort_LTT_glibc_Test == 1
    double QuickSort_LTT_glibc_SortSUM = 0;
#endif
#if QuickSort_LTT_libstdcpp_Test == 1
    double QuickSort_LTT_libstdcpp_SortSUM = 0;
#endif
#if QuickSort_glibc_Test == 1
    double QuickSort_glibc_SortSUM = 0;
#endif
#if SimpleSelectionSort_Test == 1
    double SimpleSelectionSort_SortSUM = 0;
#endif
#if HeapSort_Test == 1
    double HeapSort_SortSUM = 0;
#endif
#if MergeSort_Recursion_Test == 1
    double MergeSort_Recursion_SortSUM = 0;
#endif
#if MergeSort_Iterative_Test == 1
    double MergeSort_Iterative_SortSUM = 0;
#endif
#if MergeSort_Inplace_Iterative_Test == 1
    double MergeSort_Inplace_Iterative_SortSUM = 0;
#endif
#if MergeSort_Inplace_Iterative_For_Int_Test == 1
    double MergeSort_Inplace_Iterative_For_Int_SortSUM = 0;
#endif
#if RadixSort_LSD_Test == 1
    double RadixSort_LSD_SortSUM = 0;
#endif
#if RadixSort_MSD_Test == 1
    double RadixSort_MSD_SortSUM = 0;
#endif
#if BucketSort_Test == 1
    double BucketSort_SortSUM = 0;
#endif
#if CountingSort_Test == 1
    double CountingSort_SortSUM = 0;
#endif
#if PigeonholeSort_Test == 1
    double PigeonholeSort_SortSUM = 0;
#endif
#if BigoSort_Test == 1
    double BigoSort_SortSUM = 0;
#endif
#if IntrospectiveSort_Test == 1
    double IntrospectiveSort_SortSUM = 0;
#endif
#if sort_Test == 1
    double sort_SortSUM = 0;
#endif
#if qsort_Test == 1
    double qsort_SortSUM = 0;
#endif


#if Mode == 1          // 顺序数组
    for (int i = 0; i < NodeNum; ++i) Temp[i] = i;
#elif Mode == 2        // 逆序数组
    for (int i = 0; i < NodeNum; ++i) Temp[i] = NodeNum - i;
#endif
    for (int counter = 1; counter <= SortCounter; ++counter)
    {
#if Mode != 1 && Mode != 2
    #if Mode == 3      // 普通随机数组
        for (int i = 0; i < NodeNum; ++i) Temp[i] = rand() % NodeNum;
    #elif Mode == 4    // 无相同元素的乱序数组
        GetTheRanddomNonRepeatingArray(Temp, NodeNum);
    #elif Mode == 5    // 有很多相同元素的数组
        for (int i = 0; i < NodeNum; ++i) Temp[i] = rand() % 100;
    #elif Mode == 6    // 16个元素的小数组
        #undef NodeNum
        #define NodeNum 16
        GetTheRanddomNonRepeatingArray(Temp, NodeNum);
    #endif
#endif
        int* Array        = (int*)malloc(NodeNum * sizeof(int));
        int* OrderedArray = (int*)malloc(NodeNum * sizeof(int));
        memcpy(OrderedArray, Temp, NodeNum * sizeof(int));
        sort(OrderedArray, OrderedArray + NodeNum);
        clock_t Start, End;


#if InsertionSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        InsertionSort(Array, NodeNum, sizeof(int), cmp);
        End                    = clock();
        InsertionSort_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "InsertionSort:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if BinaryInsertionSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        BinaryInsertionSort(Array, NodeNum, sizeof(int), cmp);
        End                          = clock();
        BinaryInsertionSort_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "BinaryInsertionSort:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if ShellInsertionSort_Hibbard_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        ShellInsertionSort_Hibbard(Array, NodeNum, sizeof(int), cmp);
        End                                 = clock();
        ShellInsertionSort_Hibbard_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "ShellInsertionSort_Hibbard:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if ShellInsertionSort_Sedgewick_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        ShellInsertionSort_Sedgewick(Array, NodeNum, sizeof(int), cmp);
        End                                   = clock();
        ShellInsertionSort_Sedgewick_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "ShellInsertionSort_Sedgewick:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if BubbleSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        BubbleSort(Array, NodeNum, sizeof(int), cmp);
        End                 = clock();
        BubbleSort_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "BubbleSort:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if BubbleSort_Fast_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        BubbleSort_Fast(Array, NodeNum, sizeof(int), cmp);
        End                      = clock();
        BubbleSort_Fast_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "BubbleSort_Fast:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if QuickSort_LTT_glibc_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        QuickSort_LTT_glibc(Array, NodeNum, sizeof(int), cmp);
        End                          = clock();
        QuickSort_LTT_glibc_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "QuickSort_LTT_glibc:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if QuickSort_LTT_libstdcpp_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        QuickSort_LTT_libstdcpp(Array, NodeNum, sizeof(int), cmp);
        End                              = clock();
        QuickSort_LTT_libstdcpp_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "QuickSort_LTT_libstdcpp:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if QuickSort_glibc_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        QuickSort_glibc(Array, NodeNum, sizeof(int), cmp);
        End                      = clock();
        QuickSort_glibc_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "QuickSort_glibc:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if SimpleSelectionSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        SimpleSelectionSort(Array, NodeNum, sizeof(int), cmp);
        End                          = clock();
        SimpleSelectionSort_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "SimpleSelectionSort:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if HeapSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        HeapSort(Array, NodeNum, sizeof(int), cmp);
        End               = clock();
        HeapSort_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "HeapSort:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if MergeSort_Recursion_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        MergeSort_Recursion(Array, NodeNum, sizeof(int), cmp);
        End                          = clock();
        MergeSort_Recursion_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Recursion:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if MergeSort_Iterative_Test == 1

        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        MergeSort_Iterative(Array, NodeNum, sizeof(int), cmp);
        End                          = clock();
        MergeSort_Iterative_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Iterative:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if MergeSort_Inplace_Iterative_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        MergeSort_Inplace_Iterative(Array, NodeNum, sizeof(int), cmp);
        End                                  = clock();
        MergeSort_Inplace_Iterative_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Inplace_Iterative:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if MergeSort_Inplace_Iterative_For_Int_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        MergeSort_Inplace_Iterative_For_Int(Array, NodeNum);
        End                                          = clock();
        MergeSort_Inplace_Iterative_For_Int_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Inplace_Iterative_For_Int:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if RadixSort_LSD_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        RadixSort_LSD(Array, NodeNum, sizeof(int), getValue);
        End                    = clock();
        RadixSort_LSD_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "RadixSort_LSD:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if RadixSort_MSD_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        RadixSort_MSD(Array, NodeNum);
        End                    = clock();
        RadixSort_MSD_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "RadixSort_MSD:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if BucketSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        BucketSort(Array, NodeNum, sizeof(int), getValue);
        End                 = clock();
        BucketSort_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "BucketSort:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if CountingSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        CountingSort(Array, NodeNum, sizeof(int), getValue);
        End                   = clock();
        CountingSort_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "CountingSort:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if PigeonholeSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        PigeonholeSort(Array, NodeNum);
        End                     = clock();
        PigeonholeSort_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "PigeonholeSort:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if BigoSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        BigoSort(Array, NodeNum, sizeof(int), cmp);
        End               = clock();
        BigoSort_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "BigoSort:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if IntrospectiveSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        IntrospectiveSort(Array, NodeNum, sizeof(int), cmp);
        End                        = clock();
        IntrospectiveSort_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "IntrospectiveSort:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if sort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        sort(Array, Array + NodeNum, cmp2);
        End           = clock();
        sort_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "sort:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if qsort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        Start = clock();
        qsort(Array, NodeNum, sizeof(int), cmp);
        End            = clock();
        qsort_SortSUM += (double)(End - Start) / CLOCKS_PER_SEC * 1000;
    #if PRINT == 1
        printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "qsort:", MAX_DOUBLELENGTH, (double)(End - Start) / CLOCKS_PER_SEC * 1000);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
    }


#if InsertionSort_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "InsertionSort平均用时:", MAX_DOUBLELENGTH, InsertionSort_SortSUM / SortCounter);
#endif
#if BinaryInsertionSort_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "BinaryInsertionSort平均用时:", MAX_DOUBLELENGTH, BinaryInsertionSort_SortSUM / SortCounter);
#endif
#if ShellInsertionSort_Hibbard_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "ShellInsertionSort_Hibbard平均用时:", MAX_DOUBLELENGTH, ShellInsertionSort_Hibbard_SortSUM / SortCounter);
#endif
#if ShellInsertionSort_Sedgewick_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "ShellInsertionSort_Sedgewick平均用时:", MAX_DOUBLELENGTH, ShellInsertionSort_Sedgewick_SortSUM / SortCounter);
#endif
#if BubbleSort_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "BubbleSort平均用时:", MAX_DOUBLELENGTH, BubbleSort_SortSUM / SortCounter);
#endif
#if BubbleSort_Fast_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "BubbleSort_Fast平均用时:", MAX_DOUBLELENGTH, BubbleSort_Fast_SortSUM / SortCounter);
#endif
#if QuickSort_LTT_glibc_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "QuickSort_LTT_glibc平均用时:", MAX_DOUBLELENGTH, QuickSort_LTT_glibc_SortSUM / SortCounter);
#endif
#if QuickSort_LTT_libstdcpp_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "QuickSort_LTT_libstdcpp平均用时:", MAX_DOUBLELENGTH, QuickSort_LTT_libstdcpp_SortSUM / SortCounter);
#endif
#if QuickSort_glibc_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "QuickSort_glibc平均用时:", MAX_DOUBLELENGTH, QuickSort_glibc_SortSUM / SortCounter);
#endif
#if SimpleSelectionSort_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "SimpleSelectionSort平均用时:", MAX_DOUBLELENGTH, SimpleSelectionSort_SortSUM / SortCounter);
#endif
#if HeapSort_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "HeapSort平均用时:", MAX_DOUBLELENGTH, HeapSort_SortSUM / SortCounter);
#endif
#if MergeSort_Recursion_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Recursion平均用时:", MAX_DOUBLELENGTH, MergeSort_Recursion_SortSUM / SortCounter);
#endif
#if MergeSort_Iterative_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Iterative平均用时:", MAX_DOUBLELENGTH, MergeSort_Iterative_SortSUM / SortCounter);
#endif
#if MergeSort_Inplace_Iterative_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Inplace_Iterative平均用时:", MAX_DOUBLELENGTH, MergeSort_Inplace_Iterative_SortSUM / SortCounter);
#endif
#if MergeSort_Inplace_Iterative_For_Int_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Inplace_Iterative_For_Int平均用时:", MAX_DOUBLELENGTH, MergeSort_Inplace_Iterative_For_Int_SortSUM / SortCounter);
#endif
#if RadixSort_LSD_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "RadixSort_LSD平均用时:", MAX_DOUBLELENGTH, RadixSort_LSD_SortSUM / SortCounter);
#endif
#if RadixSort_MSD_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "RadixSort_MSD平均用时:", MAX_DOUBLELENGTH, RadixSort_MSD_SortSUM / SortCounter);
#endif
#if BucketSort_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "BucketSort平均用时:", MAX_DOUBLELENGTH, BucketSort_SortSUM / SortCounter);
#endif
#if CountingSort_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "CountingSort平均用时:", MAX_DOUBLELENGTH, CountingSort_SortSUM / SortCounter);
#endif
#if PigeonholeSort_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "PigeonholeSort平均用时:", MAX_DOUBLELENGTH, PigeonholeSort_SortSUM / SortCounter);
#endif
#if BigoSort_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "BigoSort平均用时:", MAX_DOUBLELENGTH, BigoSort_SortSUM / SortCounter);
#endif
#if IntrospectiveSort_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "IntrospectiveSort平均用时:", MAX_DOUBLELENGTH, IntrospectiveSort_SortSUM / SortCounter);
#endif
#if sort_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "sort平均用时:", MAX_DOUBLELENGTH, sort_SortSUM / SortCounter);
#endif
#if qsort_Test == 1
    printf("%-*s %*.4lf ms\n", MAX_STRINGSLENGTH, "qsort平均用时:", MAX_DOUBLELENGTH, qsort_SortSUM / SortCounter);
#endif
    printf("cmp_NUM =    %lf\n", (double)CMPNUM / SortCounter / NodeNum);
}
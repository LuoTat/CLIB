#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include "SortingAlgorithm.h"

using namespace std;
#define MAX_STRINGSLENGTH                        (36)
#define MAX_DOUBLELENGTH                         (12)


/*
    Mode == 1 //顺序数组
    Mode == 2 //逆序数组
    Mode == 3 //普通随机数组
    Mode == 4 //无相同元素的乱序数组
    Mode == 5 //有很多相同元素的数组
    Mode == 6 //16个元素的小数组
*/
#define NodeNum                                  1000000
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
#define QuickSort_LTT_glibc_Test                 0
#define QuickSort_LTT_libstdcpp_Test             0
#define QuickSort_LTT_libstdcpp_int_Test         0
#define QuickSort_glibc_Test                     0
#define QuickSort_glibc_int_Test                 0
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

inline static int getValue(const void* a) { return *(int*)a; }

inline static int cmp(const void* a, const void* b)
{
    ++CMPNUM;
    return *(int*)a - *(int*)b;
}

inline static bool cmp2(int a, int b)
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
#if QuickSort_LTT_libstdcpp_int_Test == 1
    double QuickSort_LTT_libstdcpp_int_SortSUM = 0;
#endif
#if QuickSort_glibc_Test == 1
    double QuickSort_glibc_SortSUM = 0;
#endif
#if QuickSort_glibc_int_Test == 1
    double QuickSort_glibc_int_SortSUM = 0;
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
        timespec Start, End;

#if InsertionSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        InsertionSort(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        InsertionSort_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "InsertionSort:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if BinaryInsertionSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        BinaryInsertionSort(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        BinaryInsertionSort_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "BinaryInsertionSort:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if ShellInsertionSort_Hibbard_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        ShellInsertionSort_Hibbard(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        ShellInsertionSort_Hibbard_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "ShellInsertionSort_Hibbard:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if ShellInsertionSort_Sedgewick_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        ShellInsertionSort_Sedgewick(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        ShellInsertionSort_Sedgewick_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "ShellInsertionSort_Sedgewick:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if BubbleSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        BubbleSort(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        BubbleSort_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "BubbleSort:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if BubbleSort_Fast_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        BubbleSort_Fast(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        BubbleSort_Fast_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "BubbleSort_Fast:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if QuickSort_LTT_glibc_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        QuickSort_LTT_glibc(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        QuickSort_LTT_glibc_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "QuickSort_LTT_glibc:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if QuickSort_LTT_libstdcpp_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        QuickSort_LTT_libstdcpp(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        QuickSort_LTT_libstdcpp_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "QuickSort_LTT_libstdcpp:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif

#if QuickSort_LTT_libstdcpp_int_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        QuickSort_LTT_libstdcpp_int(Array, NodeNum);
        clock_gettime(CLOCK_REALTIME, &End);
        QuickSort_LTT_libstdcpp_int_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "QuickSort_LTT_libstdcpp_int:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif

#if QuickSort_glibc_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        QuickSort_glibc(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        QuickSort_glibc_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "QuickSort_glibc:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if QuickSort_glibc_int_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        QuickSort_glibc_int(Array, NodeNum);
        clock_gettime(CLOCK_REALTIME, &End);
        QuickSort_glibc_int_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "QuickSort_glibc:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if SimpleSelectionSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        SimpleSelectionSort(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        SimpleSelectionSort_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "SimpleSelectionSort:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if HeapSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        HeapSort(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        HeapSort_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "HeapSort:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if MergeSort_Recursion_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        MergeSort_Recursion(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        MergeSort_Recursion_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Recursion:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if MergeSort_Iterative_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        MergeSort_Iterative(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        MergeSort_Iterative_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Iterative:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if MergeSort_Inplace_Iterative_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        MergeSort_Inplace_Iterative(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        MergeSort_Inplace_Iterative_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Inplace_Iterative:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if MergeSort_Inplace_Iterative_For_Int_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        MergeSort_Inplace_Iterative_For_Int(Array, NodeNum);
        clock_gettime(CLOCK_REALTIME, &End);
        MergeSort_Inplace_Iterative_For_Int_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Inplace_Iterative_For_Int:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if RadixSort_LSD_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        RadixSort_LSD(Array, NodeNum, sizeof(int), getValue);
        clock_gettime(CLOCK_REALTIME, &End);
        RadixSort_LSD_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "RadixSort_LSD:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if RadixSort_MSD_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        RadixSort_MSD(Array, NodeNum);
        clock_gettime(CLOCK_REALTIME, &End);
        RadixSort_MSD_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "RadixSort_MSD:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if BucketSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        BucketSort(Array, NodeNum, sizeof(int), getValue);
        clock_gettime(CLOCK_REALTIME, &End);
        BucketSort_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "BucketSort:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if CountingSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        CountingSort(Array, NodeNum, sizeof(int), getValue);
        clock_gettime(CLOCK_REALTIME, &End);
        CountingSort_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "CountingSort:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if PigeonholeSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        PigeonholeSort(Array, NodeNum);
        clock_gettime(CLOCK_REALTIME, &End);
        PigeonholeSort_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "PigeonholeSort:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if BigoSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        BigoSort(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        BigoSort_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "BigoSort:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if IntrospectiveSort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        IntrospectiveSort(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        IntrospectiveSort_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "IntrospectiveSort:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if sort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        sort(Array, Array + NodeNum, cmp2);
        clock_gettime(CLOCK_REALTIME, &End);
        sort_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "sort:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
#if qsort_Test == 1
        memcpy(Array, Temp, NodeNum * sizeof(int));
        clock_gettime(CLOCK_REALTIME, &Start);
        qsort(Array, NodeNum, sizeof(int), cmp);
        clock_gettime(CLOCK_REALTIME, &End);
        qsort_SortSUM += (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0;
    #if PRINT == 1
        printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "qsort:", MAX_DOUBLELENGTH, (End.tv_sec - Start.tv_sec) * 1000 + (End.tv_nsec - Start.tv_nsec) / 1000000.0);
    #endif
        IsOrdered(Array, OrderedArray, NodeNum);
#endif
    }


#if InsertionSort_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "InsertionSort", MAX_DOUBLELENGTH, InsertionSort_SortSUM / SortCounter);
#endif
#if BinaryInsertionSort_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "BinaryInsertionSort", MAX_DOUBLELENGTH, BinaryInsertionSort_SortSUM / SortCounter);
#endif
#if ShellInsertionSort_Hibbard_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "ShellInsertionSort_Hibbard", MAX_DOUBLELENGTH, ShellInsertionSort_Hibbard_SortSUM / SortCounter);
#endif
#if ShellInsertionSort_Sedgewick_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "ShellInsertionSort_Sedgewick", MAX_DOUBLELENGTH, ShellInsertionSort_Sedgewick_SortSUM / SortCounter);
#endif
#if BubbleSort_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "BubbleSort", MAX_DOUBLELENGTH, BubbleSort_SortSUM / SortCounter);
#endif
#if BubbleSort_Fast_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "BubbleSort_Fast", MAX_DOUBLELENGTH, BubbleSort_Fast_SortSUM / SortCounter);
#endif
#if QuickSort_LTT_glibc_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "QuickSort_LTT_glibc", MAX_DOUBLELENGTH, QuickSort_LTT_glibc_SortSUM / SortCounter);
#endif
#if QuickSort_LTT_libstdcpp_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "QuickSort_LTT_libstdcpp", MAX_DOUBLELENGTH, QuickSort_LTT_libstdcpp_SortSUM / SortCounter);
#endif
#if QuickSort_LTT_libstdcpp_int_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "QuickSort_LTT_libstdcpp_int", MAX_DOUBLELENGTH, QuickSort_LTT_libstdcpp_int_SortSUM / SortCounter);
#endif
#if QuickSort_glibc_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "QuickSort_glibc", MAX_DOUBLELENGTH, QuickSort_glibc_SortSUM / SortCounter);
#endif
#if QuickSort_glibc_int_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "QuickSort_glibc_int", MAX_DOUBLELENGTH, QuickSort_glibc_int_SortSUM / SortCounter);
#endif
#if SimpleSelectionSort_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "SimpleSelectionSort", MAX_DOUBLELENGTH, SimpleSelectionSort_SortSUM / SortCounter);
#endif
#if HeapSort_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "HeapSort", MAX_DOUBLELENGTH, HeapSort_SortSUM / SortCounter);
#endif
#if MergeSort_Recursion_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Recursion", MAX_DOUBLELENGTH, MergeSort_Recursion_SortSUM / SortCounter);
#endif
#if MergeSort_Iterative_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Iterative", MAX_DOUBLELENGTH, MergeSort_Iterative_SortSUM / SortCounter);
#endif
#if MergeSort_Inplace_Iterative_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Inplace_Iterative", MAX_DOUBLELENGTH, MergeSort_Inplace_Iterative_SortSUM / SortCounter);
#endif
#if MergeSort_Inplace_Iterative_For_Int_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "MergeSort_Inplace_Iterative_For_Int", MAX_DOUBLELENGTH, MergeSort_Inplace_Iterative_For_Int_SortSUM / SortCounter);
#endif
#if RadixSort_LSD_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "RadixSort_LSD", MAX_DOUBLELENGTH, RadixSort_LSD_SortSUM / SortCounter);
#endif
#if RadixSort_MSD_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "RadixSort_MSD", MAX_DOUBLELENGTH, RadixSort_MSD_SortSUM / SortCounter);
#endif
#if BucketSort_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "BucketSort", MAX_DOUBLELENGTH, BucketSort_SortSUM / SortCounter);
#endif
#if CountingSort_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "CountingSort", MAX_DOUBLELENGTH, CountingSort_SortSUM / SortCounter);
#endif
#if PigeonholeSort_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "PigeonholeSort", MAX_DOUBLELENGTH, PigeonholeSort_SortSUM / SortCounter);
#endif
#if BigoSort_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "BigoSort", MAX_DOUBLELENGTH, BigoSort_SortSUM / SortCounter);
#endif
#if IntrospectiveSort_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "IntrospectiveSort", MAX_DOUBLELENGTH, IntrospectiveSort_SortSUM / SortCounter);
#endif
#if sort_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "sort", MAX_DOUBLELENGTH, sort_SortSUM / SortCounter);
#endif
#if qsort_Test == 1
    printf("%-*s平均用时: %*lf ms\n", MAX_STRINGSLENGTH, "qsort", MAX_DOUBLELENGTH, qsort_SortSUM / SortCounter);
#endif
    printf("cmp_NUM =    %lf\n", (double)CMPNUM / SortCounter / NodeNum);
}
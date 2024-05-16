#pragma once
#include "Predefined.h"


#ifdef __cplusplus
extern "C"
{
#endif
    //###########基于插入##########
    // 插入排序
    void InsertionSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    // 折半插入
    void BinaryInsertionSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    // 希尔排序
    void ShellInsertionSort_Hibbard(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    void ShellInsertionSort_Sedgewick(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    //###########基于交换##########
    // 冒泡排序
    void BubbleSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    void BubbleSort_Fast(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    // 快速排序
    void QuickSort_LTT_glibc(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    void QuickSort_LTT_libstdcpp(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    void QuickSort_glibc(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    //###########基于选择##########
    // 简单选择排序
    void SimpleSelectionSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    // 堆排序
    void HeapSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    void PartialSort(void* Base, size_t k, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    //###########基于归并##########
    // 2-路归并排序
    void MergeSort_Recursion(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    void MergeSort_Iterative(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    void MergeSort_Inplace_Iterative(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    void MergeSort_Inplace_Iterative_For_Int(int* Base, size_t NumOfElements);
    // k路归并排序
    //  Todo
    //
    ////////////

    // 基数排序
    void RadixSort_LSD(void* Base, size_t NumOfElements, size_t SizeOfElements, ValueFunction GetValue);
    void RadixSort_MSD(int* Base, size_t NumOfElements);
    // 桶排序
    void BucketSort(void* Base, size_t NumOfElements, size_t SizeOfElements, ValueFunction GetValue);
    // 计数排序
    void CountingSort(void* Base, size_t NumOfElements, size_t SizeOfElements, ValueFunction GetValue);
    // 鸽巢排序
    void PigeonholeSort(int* Base, size_t NumOfElements);
    // 猴子排序
    void BigoSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
    // 内省式排序
    void IntrospectiveSort(void* Base, size_t NumOfElements, size_t SizeOfElements, CompareFunction Comparator);
#ifdef __cplusplus
}
#endif
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct Iterator Iterator;
    typedef int (*MoveNextFunction)(Iterator* const IP);
    typedef int (*HasNextFunction)(const Iterator* const IP);
    typedef void (*ResetFunction)(Iterator* const IP);
    typedef void (*BackToFirstFunction)(Iterator* const IP);

    struct Iterator
    {
        void*               Collection;    //用来存储迭代器所迭代的集合
        void*               Current;       //用来存储当前迭代到的元素
        MoveNextFunction    MoveNext;
        HasNextFunction     HashNext;
        ResetFunction       Reset;
        BackToFirstFunction BackToFirst;
    };

    // 专门用来返回空迭代器
    extern Iterator Iterator_NULL;
    void            InitIterator(Iterator* IP, void* Collection, MoveNextFunction MoveNext, HasNextFunction HashNext, ResetFunction Reset, BackToFirstFunction BackToFirst);
#ifdef __cplusplus
}
#endif
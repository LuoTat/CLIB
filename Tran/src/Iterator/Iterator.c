#include "Iterator.h"
#include <stdlib.h>

Iterator Iterator_NULL = {NULL, NULL, NULL, NULL, NULL, NULL};

void InitIterator(Iterator* IP, void* Collection, MoveNextFunction MoveNext, HasNextFunction HashNext, ResetFunction Reset, BackToFirstFunction BackToFirst)
{
    IP->Current     = NULL;    // 初始化迭代器
    IP->Collection  = Collection;
    IP->MoveNext    = MoveNext;
    IP->HashNext    = HashNext;
    IP->Reset       = Reset;
    IP->BackToFirst = BackToFirst;
}

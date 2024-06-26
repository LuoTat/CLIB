#pragma once
/*
    此哈希表的实现是基于链地址法的哈希表
    哈希表内存储的是键值对，键和值都是void*类型的指针
    故用户在使用此哈希表时，需要自己实现哈希函数，以及判断两个键和值是否相等的函数
    否则，可以使用默认的哈希函数和判断两个键是否相等的函数
    使用时不能放入临时变量的地址，因为临时变量会在函数结束时被销毁
*/

#include "../src/HashMap/HashMap.h"    // IWYU pragma: export

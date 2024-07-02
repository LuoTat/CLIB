#pragma once
// 这个文件定义了一些通用的工具宏

// 分支预测宏
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

// 用于定义内联函数
#ifndef LTT_inline
    #ifdef _MSC_VER
        #define LTT_inline __inline
    #else
        #define LTT_inline inline
    #endif
#endif /* LTT_inline */

// 用于定义不使用的函数
#ifndef LTT_unused
    #if (defined __clang__ && __clang_major__ >= 3) || (defined __GNUC__ && __GNUC__ >= 3)
        #define LTT_unused __attribute__((__unused__))
    #else
        #define LTT_unused
    #endif
#endif /* LTT_unused */

// 用于定义函数返回的错误码
typedef enum CODE
{
    Success,                  // 成功
    MemoryAllocationError,    // 内存分配错误
    MemoryOverflow,           // 内存溢出
    NullPointerAccess,        // 访问NULL指针
    ArrayIndexOutOfRange,     // 数组越界
    KeyDuplicated,            // 键重复
    InvalidIndex,             // 无效索引
    InvalidArgument,          // 参数错误
    InvalidOperation,         // 无效操作
    UnknownError              // 未知错误
} CODE;
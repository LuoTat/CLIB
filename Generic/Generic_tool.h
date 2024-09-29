#pragma once
// This file defines some commonly used macros in the project

// Branch prediction macros
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

// Used to define unused variables and functions
#ifndef LTT_UNUSED
    #if (defined __GNUC__ && __GNUC__ >= 3) || (defined __clang__ && __clang_major__ >= 3)
        #define LTT_UNUSED __attribute__((__unused__))
    #else
        #define LTT_UNUSED
    #endif
#endif /* LTT_UNUSED */

// Used to define error codes for function returns
typedef enum CODE
{
    Success,                  // Success
    MemoryAllocationError,    // Memory allocation error
    MemoryOverflow,           // Memory overflow
    NullPointerAccess,        // Accessing NULL pointer
    ArrayIndexOutOfRange,     // Array index out of bounds
    KeyDuplicated,            // Key duplicated
    InvalidIndex,             // Invalid index
    InvalidArgument,          // Invalid argument
    InvalidOperation,         // Invalid operation
    UnknownError              // Unknown error
} CODE;

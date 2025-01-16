# CLIB

<p align="center">A user-friendly and high-performance generic collection library for C, based on macros, header-only</p>

[**English**](README.md)&nbsp;&nbsp;&nbsp;[**简体中文**](README_CN.md)

## Contents

-   [Introduction](#introduction)
-   [Installation](#installation)
-   [Usage](#usage)
-   [Project Structure](#project-structure)
-   [API Documentation](#api-documentation)

## [Introduction](#table-of-contents)

CLIB is a generic collection library developed entirely in C. It primarily uses macros to generate target functions. The main goal of the project is to provide C developers with a user-friendly and high-performance generic collection library.

## [Installation](#table-of-contents)

If you only want to quickly use the CLIB library, simply download the `Generic` folder and add it to your header file search path.

If you want to conduct performance tests or unit tests, you need to clone the entire project...

## [Usage](#table-of-contents)

Here are some specific examples:

### ArrayList

`ArrayList` is a dynamically resizable array.

```c
// Include the corresponding header file ArrayList_G.h
#include "ArrayList_G.h"

/*
 * The INTCMP macro defines an equality comparison function for the int type.
 * Different collection types may require different functions.
 * For example, a custom type HashSet requires both an equality comparison function and a hash function.
 */
#define INTCMP(a, b) ((a) == (b))

/*
 * The LTT_ARRAYLIST_INIT macro generates all the structures and functions for the collection class of the corresponding type.
 * The INT parameter uniquely identifies the structure and functions of the generated collection class.
 * The int parameter specifies the concrete type for instantiating the collection class.
 * The INTCMP parameter is the equality comparison function provided.
 */
LTT_ARRAYLIST_INIT(INT, int, INTCMP)

int main()
{
    // The ArrayList macro declares an ArrayList collection class identified by INT.
    ArrayList(INT) ArrayList_G;
    // The ArrayList_Init macro initializes the collection.
    ArrayList_Init(INT, &ArrayList_G);
    for (int i = 0; i < 10; ++i)
    {
        // The ArrayList_AddFirst macro adds an element to the head of ArrayList_G.
        ArrayList_AddFirst(INT, &ArrayList_G, i);
    }
}
```

### HashSet

`HashSet` is an unordered set implemented with a hash table.

```c
// Include the corresponding header file HashSet_G.h
#include "HashSet_G.h"

// Declare a Student struct to be stored in the HashSet.
typedef struct Student
{
    int ID;
    int age;
} Student;

/*
 *
 * The LTT_HASHSET_INIT_DEFAULT macro generates all the structures and functions for the collection class of the corresponding type.
 * This macro also provides default equality and hash functions for custom types.
 */
LTT_HASHSET_INIT_DEFAULT(STU, Student);

int main()
{
    // The HashSet macro declares a HashSet collection class identified by STU.
    HashSet(STU) HashSet_G;
    // The HashSet_Init macro initializes the collection.
    HashSet_Init(STU, &HashSet_G);
    Student stu;
    for (int i = 0; i < 10; ++i)
    {
        stu.ID  = i;
        stu.age = i % 18;
        // The HashSet_Add macro adds an element to HashSet_G.
        HashSet_Add(STU, &HashSet_G, stu);
    }
}
```

## [Project Structure](#table-of-contents)

-   **Generic**: Contains all the header files for CLIB's generic collection library.
-   **benchmark**: Uses the GoogleBenchmark library to compare the performance of CLIB with the STL collection classes in C++.
-   **test**: Uses the GoogleTest library to perform unit testing on CLIB.

Some legacy files and folders remain and will be cleaned up gradually.

## [API Documentation](#table-of-contents)

For the complete Chinese API documentation, click [here](api_reference_CN.md).

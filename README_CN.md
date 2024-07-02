# CLIB

<p align="center">一套易用且高性能的 C 泛型集合类库，基于宏，只有头文件</p>

## 目录

-   [简介](#简介)
-   [安装方法](#安装方法)
-   [使用方法](#使用方法)
-   [项目结构](#项目结构)
-   [API 文档](#api-文档)

## [简介](#目录)

CLIB 是一个完全使用 C 语言开发的泛型集合类库，主要使用了宏控制来生成目标函数。整个项目的主要目的是为了给 C 语言开发者提供一套易用且高性能的泛型集合类库。

## [安装方法](#目录)

如果只想快速使用 CLIB 库，可以只下载 Generic 文件夹，然后将 Generic 文件夹添加到头文件的搜索范围内即可使用。

如果想要进行性能测试或单元测试，则需要将整个项目克隆下来......

## [使用方法](#目录)

下面列举几个具体的用例

### ArrayList

`ArrayList` 是动态可变长数组

```c
// 引入对应的头文件 ArrayList_G.h
#include "ArrayList_G.h"

/*
 * INTCMP宏定义了一个针对int类型的一个判等函数
 * 不同的集合类会要求你提供不同的函数
 * 比如自定义类型的HashSet就需要判等函数以及哈希函数
 */
#define INTCMP(a, b) ((a) == (b))

/*
 * LTT_ARRAYLIST_INIT宏是用来生成对应类型的集合类的所有结构体以及函数
 * 这里的 INT 参数是用来唯一标识所生成的集合类的结构体以及函数的
 * int 参数是用来实例化集合类的具体类型
 * INTCMP 参数则是传入的判等函数
 */
LTT_ARRAYLIST_INIT(INT, int, INTCMP)

int main()
{
    // ArrayList宏是用来声明以INT标识的ArrayList这个集合类的
    ArrayList(INT) ArrayList_G;
    // ArrayList_Init宏用来初始化
    ArrayList_Init(INT, &ArrayList_G);
    for (int i = 0; i < 10; ++i)
    {
        // ArrayList_AddFirst宏用来向ArrayList_G的头部添加元素
        ArrayList_AddFirst(INT, &ArrayList_G, i);
    }
}
```

### HashSet

`HashSet` 是基于哈希表实现的无序 Set

```c
// 引入对应的头文件 HashSet_G.h
#include "HashSet_G.h"

// 声明一个Student类，用来放入HashSet中
typedef struct Student
{
    int ID;
    int age;
} Student;

/*
 *
 * LTT_HASHSET_INIT_DEFAULT宏是用来生成对应类型的集合类的所有结构体以及函数
 * 并且这个宏还给用户自定义类型提供一套默认的判等函数和哈希函数
 */
LTT_HASHSET_INIT_DEFAULT(STU, Student);

int main()
{
    // HashSet宏是用来声明以STU标识的HashSet这个集合类的
    HashSet(STU) HashSet_G;
    // HashSet_Init宏用来初始化
    HashSet_Init(STU, &HashSet_G);
    Student stu;
    for (int i = 0; i < 10; ++i)
    {
        stu.ID  = i;
        stu.age = i % 18;
        // HashSet_Add宏用来向HashSet_G中添加元素
        HashSet_Add(STU, &HashSet_G, stu);
    }
}
```

## [项目结构](#目录)

-   **benchmark** 使用 GoogleBenchmark 库来对 CLIB 和 C++的 STL 中的集合做对比
-   **Generic** 存放 CLIB 库所有的泛型集合类库的头文件
-   **test** 使用 GoogleTest 库来对 CLIB 做单元测试

目前还有一些遗留的文件及文件夹，后续将慢慢清理

## [API 文档](#目录)

完整的 API 文档请点击[这里](api_reference_CN.md).

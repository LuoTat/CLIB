# CLIB API 文档

> **Note:**<br> 关于 API 文档中对应集合类的函数的规约<br> 还可以参考对应的集合类的头文件中的注释<br>

```c
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
```

## 集合类

1. [ArrayDeque](#arraydeque)
2. [ArrayList](#arraylist)
3. [ArrayStack](#arraystack)
4. [HashSet](#hashset)
5. [LinkedList](#linkedlist)

## [ArrayDeque](#集合类)

`ArrayDeque` 是一个双端环形队列，支持在头部和尾部进行插入和删除操作。使用连续的数组来实现。

```c
LTT_ARRAYDEQUE_INIT(NAME, TYPE, Equals_Function)
```

实例化一个`TYPE`类型的`ArrayDeque`集合类，`NAME`是用来唯一标识所生成的集合类的结构体以及函数，`Equals_Function`是传入的判等函数。

---

```c
ArrayDeque(NAME) ArrayDeque_G;
```

声明一个`NAME`标识的`ArrayDeque`集合类。

---

```c
CODE ArrayDeque_Init(NAME, ArrayDeque_G);
```

初始化一个`ArrayDeque`集合类。

---

```c
CODE ArrayDeque_AddFirst(NAME, ArrayDeque_G, Element);
```

向`ArrayDeque`集合类的头部添加元素`Element`。

---

```c
CODE ArrayDeque_AddLast(NAME, ArrayDeque_G, Element);
```

向`ArrayDeque`集合类的尾部添加元素`Element`。

---

```c
CODE ArrayDeque_DeleteFirst(NAME, ArrayDeque_G, Result);
```

删除`ArrayDeque`集合类的头部元素,并将删除的元素存储到`Result`中。

---

```c
CODE ArrayDeque_DeleteLast(NAME, ArrayDeque_G, Result);
```

删除`ArrayDeque`集合类的尾部元素,并将删除的元素存储到`Result`中。

---

```c
CODE ArrayDeque_GetFirst(NAME, ArrayDeque_G, Result);
```

获取`ArrayDeque`集合类的头部元素,并将元素存储到`Result`中。

---

```c
CODE ArrayDeque_GetLast(NAME, ArrayDeque_G, Result);
```

获取`ArrayDeque`集合类的尾部元素,并将元素存储到`Result`中。

---

```c
int ArrayDeque_GetSize(NAME, ArrayDeque_G);
```

获取`ArrayDeque`集合类的元素的个数。。

---

```c
bool ArrayDeque_IsEmpty(NAME, ArrayDeque_G);
```

判断`ArrayDeque`集合类是否为空。

---

```c
bool ArrayDeque_Contains(NAME, ArrayDeque_G, Element);
```

判断`ArrayDeque`集合类是否包含元素`Element`。

---

```c
void ArrayDeque_Clear(NAME, ArrayDeque_G);
```

清空`ArrayDeque`集合类。

---

```c
void ArrayDeque_Destroy(NAME, ArrayDeque_G);
```

销毁`ArrayDeque`集合类。

## [ArrayList](#集合类)

`ArrayList` 是动态可变长数组。使用连续的数组来实现。

```c
LTT_ARRAYLIST_INIT(NAME, TYPE, Equals_Function)
```

实例化一个`TYPE`类型的`ArrayList`集合类，`NAME`是用来唯一标识所生成的集合类的结构体以及函数，`Equals_Function`是传入的判等函数。

---

```c
ArrayList(NAME) ArrayList_G;
```

声明一个`NAME`标识的`ArrayList`集合类。

---

```c
CODE ArrayList_Init(NAME, ArrayList_G);
```

初始化一个`ArrayList`集合类。

---

```c
CODE ArrayList_AddFirst(NAME, ArrayList_G, Element);
```

向`ArrayList`集合类的头部添加元素`Element`。

---

```c
CODE ArrayList_AddLast(NAME, ArrayList_G, Element);
```

向`ArrayList`集合类的尾部添加元素`Element`。

---

```c
CODE ArrayList_AddIndex(NAME, ArrayList_G, Index, Element);
```

向`ArrayList`集合类的指定位置`Index`添加元素`Element`。

---

```c
CODE ArrayList_DeleteFirst(NAME, ArrayList_G, Result);
```

删除`ArrayList`集合类的头部元素,并将删除的元素存储到`Result`中。

---

```c
CODE ArrayList_DeleteLast(NAME, ArrayList_G, Result);
```

删除`ArrayList`集合类的尾部元素,并将删除的元素存储到`Result`中。

---

```c
CODE ArrayList_DeleteIndex(NAME, ArrayList_G, Index, Result);
```

删除`ArrayList`集合类的指定位置`Index`的元素,并将删除的元素存储到`Result`中。

---

```c
CODE ArrayList_SetIndex(NAME, ArrayList_G, Result);
```

设置`ArrayList`集合类的指定位置`Index`的元素,并将原来的元素存储到`Result`中。

---

```c
CODE ArrayList_GetFirst(NAME, ArrayList_G, Result);
```

获取`ArrayList`集合类的头部元素,并将元素存储到`Result`中。

---

```c
CODE ArrayList_GetLast(NAME, ArrayList_G, Result);
```

获取`ArrayList`集合类的尾部元素,并将元素存储到`Result`中。

---

```c
CODE ArrayList_GetIndex(NAME, ArrayList_G, Index, Result);
```

获取`ArrayList`集合类的指定位置`Index`的元素,并将元素存储到`Result`中。

---

```c
int ArrayList_IndexOf(NAME, ArrayList_G, Element);
```

获取`ArrayList`集合类的元素`Element`的正向索引。

---

```c
int ArrayList_LastIndexOf(NAME, ArrayList_G, Element);
```

获取`ArrayList`集合类的元素`Element`的反向索引。

---

```c
int ArrayList_GetSize(NAME, ArrayList_G);
```

获取`ArrayList`集合类的元素的个数。

---

```c
bool ArrayList_IsEmpty(NAME, ArrayList_G);
```

判断`ArrayList`集合类是否为空。

---

```c
bool ArrayList_Contains(NAME, ArrayList_G, Element);
```

判断`ArrayList`集合类是否包含元素`Element`。

---

```c
void ArrayList_Clear(NAME, ArrayList_G);
```

清空`ArrayList`集合类。

---

```c
void ArrayList_Destroy(NAME, ArrayList_G);
```

销毁`ArrayList`集合类。

## [ArrayStack](#集合类)

`ArrayStack` 是一个基于数组实现的栈。

```c
LTT_ARRAYSTACK_INIT(NAME, TYPE, Equals_Function)
```

实例化一个`TYPE`类型的`ArrayStack`集合类，`NAME`是用来唯一标识所生成的集合类的结构体以及函数，`Equals_Function`是传入的判等函数。

---

```c
ArrayStack(NAME) ArrayStack_G;
```

声明一个`NAME`标识的`ArrayStack`集合类。

---

```c
CODE ArrayStack_Init(NAME, ArrayStack_G);
```

初始化一个`ArrayStack`集合类。

---

```c
CODE ArrayStack_Push(NAME, ArrayStack_G, Element);
```

向`ArrayStack`集合类的栈顶添加元素`Element`。

---

```c
CODE ArrayStack_Pop(NAME, ArrayStack_G, Result);
```

从`ArrayStack`集合类的栈顶删除元素，并将删除的元素存储到`Result`中。

---

```c
CODE ArrayStack_Peek(NAME, ArrayStack_G, Result);
```

获取`ArrayStack`集合类的栈顶元素，并将元素存储到`Result`中。

---

```c
int ArrayStack_GetSize(NAME, ArrayStack_G);
```

获取`ArrayStack`集合类的元素的个数。

---

```c
bool ArrayStack_IsEmpty(NAME, ArrayStack_G);
```

判断`ArrayStack`集合类是否为空。

---

```c

void ArrayStack_Clear(NAME, ArrayStack_G);
```

清空`ArrayStack`集合类。

---

```c
void ArrayStack_Destroy(NAME, ArrayStack_G);
```

销毁`ArrayStack`集合类。

## [HashSet](#集合类)

`HashSet` 是基于哈希表实现的无序 Set。

```c
LTT_HASHSET_INIT(NAME, TYPE, Equals_Function, Hash_Function)
```

实例化一个`TYPE`类型的`HashSet`集合类，`NAME`是用来唯一标识所生成的集合类的结构体以及函数，`Equals_Function` 是传入的判等函数，`Hash_Function`是传入的哈希函数。

---

```c
HashSet(NAME) HashSet_G;
```

声明一个`NAME`标识的`HashSet`集合类。

---

```c
CODE HashSet_Init(NAME, HashSet_G);
```

初始化一个`HashSet`集合类。

---

```c

CODE HashSet_Add(NAME, HashSet_G, Element);
```

向`HashSet`集合类中添加元素`Element`。

---

```c
bool HashSet_Delete(NAME, HashSet_G, Element, Result);
```

删除`HashSet`集合类中的元素`Element`，并将删除的元素存储到`Result`中。

---

```c
bool HashSet_Contains(NAME, HashSet_G, Element);
```

判断`HashSet`集合类是否包含元素`Element`。

---

```c
int HashSet_GetSize(NAME, HashSet_G);
```

获取`HashSet`集合类的元素的个数。

---

```c
bool HashSet_IsEmpty(NAME, HashSet_G);
```

判断`HashSet`集合类是否为空。

---

```c
void HashSet_Clear(NAME, HashSet_G);
```

清空`HashSet`集合类。

---

```c
void HashSet_Destroy(NAME, HashSet_G);
```

销毁`HashSet`集合类。

## [LinkedList](#集合类)

`LinkedList` 是一个双向链表。

```c
LTT_LINKEDLIST_INIT(NAME, TYPE, Equals_Function)
```

实例化一个`TYPE`类型的`LinkedList`集合类，`NAME`是用来唯一标识所生成的集合类的结构体以及函数，`Equals_Function`是传入的判等函数。

---

```c
LinkedList(NAME) LinkedList_G;
```

声明一个`NAME`标识的`LinkedList`集合类。

---

```c
CODE LinkedList_Init(NAME, LinkedList_G);
```

初始化一个`LinkedList`集合类。

---

```c
CODE LinkedList_AddFirst(NAME, LinkedList_G, Element);
```

向`LinkedList`集合类的头部添加元素`Element`。

---

```c
CODE LinkedList_AddLast(NAME, LinkedList_G, Element);
```

向`LinkedList`集合类的尾部添加元素`Element`。

---

```c
CODE LinkedList_AddIndex(NAME, LinkedList_G, Index, Element);
```

向`LinkedList`集合类的指定位置`Index`添加元素`Element`。

---

```c
CODE LinkedList_DeleteFirst(NAME, LinkedList_G, Result);
```

删除`LinkedList`集合类的头部元素,并将删除的元素存储到`Result`中。

---

```c
CODE LinkedList_DeleteLast(NAME, LinkedList_G, Result);
```

删除`LinkedList`集合类的尾部元素,并将删除的元素存储到`Result`中。

---

```c
CODE LinkedList_DeleteIndex(NAME, LinkedList_G, Index, Result);
```

删除`LinkedList`集合类的指定位置`Index`的元素,并将删除的元素存储到`Result`中。

---

```c
CODE LinkedList_SetFirst(NAME, LinkedList_G, Result);
```

设置`LinkedList`集合类的头部元素,并将原来的元素存储到`Result`中。

---

```c
CODE LinkedList_SetLast(NAME, LinkedList_G, Result);
```

设置`LinkedList`集合类的尾部元素,并将原来的元素存储到`Result`中。

---

```c
CODE LinkedList_SetIndex(NAME, LinkedList_G, Index, Result);
```

设置`LinkedList`集合类的指定位置`Index`的元素,并将原来的元素存储到`Result`中。

---

```c
CODE LinkedList_GetFirst(NAME, LinkedList_G, Result);
```

获取`LinkedList`集合类的头部元素,并将元素存储到`Result`中。

---

```c
CODE LinkedList_GetLast(NAME, LinkedList_G, Result);
```

获取`LinkedList`集合类的尾部元素,并将元素存储到`Result`中。

---

```c
CODE LinkedList_GetIndex(NAME, LinkedList_G, Index, Result);
```

获取`LinkedList`集合类的指定位置`Index`的元素,并将元素存储到`Result`中。

---

```c
int LinkedList_IndexOf(NAME, LinkedList_G, Element);
```

获取`LinkedList`集合类的元素`Element`的正向索引。

---

```c
int LinkedList_LastIndexOf(NAME, LinkedList_G, Element);
```

获取`LinkedList`集合类的元素`Element`的反向索引。

---

```c
int LinkedList_GetSize(NAME, LinkedList_G);
```

获取`LinkedList`集合类的元素的个数。

---

```c
bool LinkedList_IsEmpty(NAME, LinkedList_G);
```

判断`LinkedList`集合类是否为空。

---

```c
bool LinkedList_Contains(NAME, LinkedList_G, Element);
```

判断`LinkedList`集合类是否包含元素`Element`。

---

```c
void LinkedList_Destroy(NAME, LinkedList_G);
```

销毁`LinkedList`集合类。

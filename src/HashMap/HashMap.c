#include "HashMap.h"
#include <limits.h>
#include "../Iterator/Iterator.h"
#include "../Predefined/Predefined.h"


#define DEFAULT_INITIAL_CAPACITY (1 << 4)
#define MAXIMUM_CAPACITY         (1 << 30)
#define DEFAULT_LOAD_FACTOR      0.75f

typedef struct HashNode
{
    int              hash;
    void*            Key;
    void*            Value;
    struct HashNode* Next;
} HashNode;

typedef struct _HashMap
{
    //哈希表的相关参数
    int    Size;          //哈希表中的节点数
    int    Capacity;      //数组的总长度
    int    Threshold;     //扩容的阈值
    float  LoadFactor;    //负载因子
    size_t KeySize;
    size_t ValueSize;
    //哈希表本身
    HashNode** Table;
    //迭代器
    Iterator Iterator;
    //用户可自定义的函数
    GetHashCode_Function GetHashCode;
    Equals_Function      Equals_Key;
    Equals_Function      Equals_Value;
} _HashMap;

void* LTT_HashMap_GetCurrent_Key(const Iterator* const IP) { return ((HashNode*)(IP->Current))->Key; }

void* LTT_HashMap_GetCurrent_Value(const Iterator* const IP) { return ((HashNode*)(IP->Current))->Value; }

inline static void LTT_HashMap_BackToFirst(Iterator* const IP)
{
    HashMap* HashMap = IP->Collection;
    for (int index = 0; index < HashMap->Capacity; ++index)
    {
        if (HashMap->Table[index] != NULL)
        {
            IP->Current = HashMap->Table[index];
            break;
        }
    }
}

inline static int LTT_HashMap_HasNext(const Iterator* const IP)
{
    HashMap*  HashMap = IP->Collection;
    HashNode* Current = IP->Current;
    if (Current->Next != NULL) return 1;    //表示当前链表还有下一个元素
    int index = Current->hash & (HashMap->Capacity - 1);
    while (++index < HashMap->Capacity)
    {
        if (HashMap->Table[index] != NULL) return 1;
    }
    return 0;
}

static int LTT_HashMap_MoveNext(Iterator* const IP)
{
    HashMap*  HashMap = IP->Collection;
    HashNode* Current = IP->Current;
    if (Current == NULL)
    {
        LTT_HashMap_BackToFirst(IP);    //迭代器自初始化成功刻起，必然有至少一个元素
        return 1;
    }
    if (Current->Next != NULL)
    {
        IP->Current = Current->Next;
        return 1;
    }
    int index = Current->hash & (HashMap->Capacity - 1);
    while (++index < HashMap->Capacity)
    {
        if (HashMap->Table[index] != NULL)
        {
            IP->Current = HashMap->Table[index];
            return 1;
        }
    }
    return 0;
}

static void LTT_HashMap_Reset(Iterator* const IP) { IP->Current = NULL; }

Iterator LTT_HashMap_GetIterator(const HashMap* const HashMap)
{
    if (HashMap->Table == NULL || HashMap->Capacity <= 0) { printf("哈希表为空!\n"); }
    return HashMap->Iterator;
}

HashMap* LTT_HashMap_New(const size_t KeySize, const size_t ValueSize, const GetHashCode_Function GetHashCode, const Equals_Function Equals_Key, const Equals_Function Equals_Value)
{
    HashMap* HMP = (HashMap*)malloc(sizeof(HashMap));
    if (HMP == NULL)
    {
        printf("内存分配失败!\n");
        return NULL;
    }
    //初始化哈希表的相关参数
    HMP->Capacity   = 0;
    HMP->Threshold  = 0;
    HMP->Size       = 0;
    HMP->LoadFactor = DEFAULT_LOAD_FACTOR;
    HMP->Table      = NULL;
    HMP->KeySize    = KeySize;
    HMP->ValueSize  = ValueSize;
    //初始化迭代器
    InitIterator(&HMP->Iterator, HMP, LTT_HashMap_MoveNext, LTT_HashMap_HasNext, LTT_HashMap_Reset, LTT_HashMap_BackToFirst);
    //用户可自定义的函数
    HMP->GetHashCode  = (GetHashCode) ? GetHashCode : LTT_DEFAULT_GetHashCode;
    HMP->Equals_Key   = (Equals_Key) ? Equals_Key : LTT_DEFAULT_Equals;
    HMP->Equals_Value = (Equals_Value) ? Equals_Value : LTT_DEFAULT_Equals;
    return HMP;
}

inline static int LTT_HashMap_hash(const HashMap* const HashMap, const void* const Key)
{
    int h = HashMap->GetHashCode(Key, HashMap->KeySize);
    return (Key == NULL) ? 0 : h ^ (h >> 16);
}

static HashNode* LTT_HashMap_MakeNode(const int hash, void* const Key, void* const Value, HashNode* Next)
{
    HashNode* HNP = (HashNode*)malloc(sizeof(HashNode));
    if (HNP == NULL)
    {
        printf("内存分配失败!\n");
        return NULL;
    }
    HNP->hash  = hash;
    HNP->Key   = Key;
    HNP->Value = Value;
    HNP->Next  = Next;
    return HNP;
}

static HashNode** LTT_HashMap_Resize(HashMap* const HashMap)
{
    HashNode** OldTab       = HashMap->Table;
    int        OldCapacity  = (OldTab == NULL) ? 0 : HashMap->Capacity;    //表示之前的数组容量
    int        OldThreshold = HashMap->Threshold;                          //表示之前的阈值
    int        NewCapacity  = 0;                                           //表示新的数组容量
    int        NewThreshold = 0;                                           //表示新的阈值
    //表示之前的数组容量不为0
    if (OldCapacity > 0)
    {
        if (OldCapacity >= MAXIMUM_CAPACITY)    //如果此时的数组容量大于最大值
        {
            HashMap->Threshold = INT_MAX;       //扩容阈值为Int类型的最大值，这种情况很少出现
            return OldTab;
        }
        //表示OldTab数组的长度没有那么大，进行扩容两倍,对阈值也进行扩容
        else if ((NewCapacity = OldCapacity << 1) < MAXIMUM_CAPACITY && OldCapacity >= DEFAULT_INITIAL_CAPACITY)
        {
            HashMap->Capacity = NewCapacity;
            NewThreshold      = OldThreshold << 1;
        }
    }
    //表示之前的容量是0但是之前的阈值却大于零，此时新的hash表长度等于此时的阈值
    else if (OldThreshold > 0) { HashMap->Capacity = NewCapacity = OldThreshold; }
    else                                                                        //表示初始化时候，采用默认的阈值=数组长度*负载因子
    {
        HashMap->Capacity = NewCapacity = DEFAULT_INITIAL_CAPACITY;
        NewThreshold                    = (int)(DEFAULT_LOAD_FACTOR * DEFAULT_INITIAL_CAPACITY);
    }
    if (NewThreshold == 0)                                                      //此时表示若新的阈值为0 就得用新容量*加载因子重新进行计算。
    {
        float ft     = (float)NewCapacity * HashMap->LoadFactor;
        NewThreshold = (NewCapacity < MAXIMUM_CAPACITY && ft < (float)MAXIMUM_CAPACITY ? (int)ft : INT_MAX);
    }
    HashMap->Threshold = NewThreshold;
    HashNode** NewTab  = (HashNode**)calloc(NewCapacity, sizeof(HashNode*));    //初始化新的hash表，并且初始化为NULL
    if (NewTab == NULL)
    {
        printf("内存分配失败");
        return NULL;
    }
    HashMap->Table = NewTab;

    //下面开始遍历旧的hash表，将之内的元素移到新的hash表中
    //其中的窍门很多
    //首先是hash表的长度是2的幂次方，所以hash表的长度-1的二进制表示为全1
    //比如假设hash表的长度没有扩容前是16，则16-1的二进制表示为
    //0000 0000 0000 0000 0000 0000 0000 1111
    //在计算索引的时候，只需要将hash值与上hash表的长度-1即可，因为此时就相当于取hash值的低4位，而这个值正好是mod运算的结果
    //然后在扩容了后，hash表的长度是32，则32-1的二进制表示为
    //0000 0000 0000 0000 0000 0000 0001 1111
    //可以看到就相当是取hash值的低5位
    //于是在扩容后，如果hash值的低第5位为0，则表示索引不变，如果为1，则表示索引为原来的索引+原来的hash表的长度
    //而判断hash值的低第5位是否为0，只需要将hash值与上hash表的长度即可，如果为0，则表示低第5位为0，如果不为0，则表示低第5位为1

    if (OldTab != NULL)                                         //表示旧的hash表不为空
    {
        for (int i = 0; i < OldCapacity; ++i)                   //遍历旧的hash表
        {
            HashNode* Temp;
            if ((Temp = OldTab[i]) != NULL)                     //表示OldTab[i]不为空
            {
                OldTab[i] = NULL;                               //将OldTab[i]置空
                if (Temp->Next == NULL)                         //表示当前只有一个元素，重新做hash散列并赋值
                {
                    NewTab[Temp->hash & (NewCapacity - 1)] = Temp;
                }
                else                                            //处理链表，使用的是尾插法
                {
                    HashNode* UnchangeHead = NULL;              //表示索引不变的链表的头节点
                    HashNode* UnChangeTail = NULL;              //表示索引不变的链表的尾节点
                    HashNode* ChangeHead   = NULL;              //表示索引改变的链表的头节点
                    HashNode* ChangeTail   = NULL;              //表示索引改变的链表的尾节点
                    HashNode* Next;
                    do {
                        Next = Temp->Next;                      //先保存下一个节点
                        if ((Temp->hash & OldCapacity) == 0)    //多出来的一位为0，表示数组索引不变
                        {
                            //如果还没有尾节点，将当前节点作为头节点
                            if (UnchangeHead == NULL) UnchangeHead = Temp;
                            //如果已经有尾节点了，将当前节点链接到尾节点的后面
                            else UnChangeTail->Next = Temp;    //链接链表
                            UnChangeTail = Temp;               //更新尾节点
                        }
                        else
                        {
                            if (ChangeHead == NULL) ChangeHead = Temp;
                            else ChangeTail->Next = Temp;      //链接链表
                            ChangeTail = Temp;
                        }
                    }
                    while ((Temp = Next) != NULL);             //如果下一个节点不为空，则继续循环
                    if (UnChangeTail != NULL)
                    {
                        UnChangeTail->Next = NULL;
                        NewTab[i]          = UnchangeHead;
                    }
                    if (ChangeTail != NULL)
                    {
                        ChangeTail->Next        = NULL;
                        NewTab[i + OldCapacity] = ChangeHead;
                    }
                }
            }
        }
    }
    free(OldTab);
    return NewTab;
}

static void* LTT_HashMap_PutVal(HashMap* const HashMap, void* const Key, void* const Value, const int hash, const bool OnlyIfAbsent)
{
    HashNode** Table;          //用来表示当前的哈希表
    HashNode*  LocatedNode;    //用来表示定位到的节点
    int        index;          //用来表示定位到的节点的索引
    int        Capacity;       //用来表示当前哈希表的长度
    int        KeySize = HashMap->KeySize;
    //采用懒加载的方式，如果当前的哈希表为空或者长度为0，则进行扩容
    if ((Table = HashMap->Table) == NULL || (Capacity = HashMap->Capacity) == 0)
    {
        Table    = LTT_HashMap_Resize(HashMap);
        Capacity = HashMap->Capacity;
    }
    //如果当前的Table不为空，且定位到的节点为空，则初始化一个新的节点并放入该Table中
    if ((LocatedNode = Table[index = (Capacity - 1) & hash]) == NULL)
    {
        if ((Table[index] = LTT_HashMap_MakeNode(hash, Key, Value, NULL)) == NULL)
        {
            printf("HashNode制作失败!\n");
            return NULL;
        }
    }
    else    //如果定位到的节点不为空，则遍历链表，如果找到了相同的Key，则替换Value，否则将该节点放入链表中
    {
        HashNode*   TempNode;
        const void* TempKey;
        //先看hash值是否相等，如果相等，再看Key，看Key的时候先看Key的地址是否相等，如果不同再比较Key的内容是否相等
        //如果Key的内容相等，则表示找到了相同的Key，此时将e指向该节点
        if (LocatedNode->hash == hash && ((TempKey = LocatedNode->Key) == Key || (Key != NULL && HashMap->Equals_Key(Key, TempKey, KeySize)))) { TempNode = LocatedNode; }
        else                                                                                           //如果Key不相等，则遍历链表
        {
            while (true)
            {
                if ((TempNode = LocatedNode->Next) == NULL)                                            //如果当前节点的下一个节点为空，则表示没有找到相同的Key，此时将TempNode指向NULL
                {
                    if ((LocatedNode->Next = LTT_HashMap_MakeNode(hash, Key, Value, NULL)) == NULL)    //将新的节点放入链表中
                    {
                        printf("HashNode制作失败");
                        return NULL;
                    }
                    break;
                }
                if (TempNode->hash == hash && ((TempKey = TempNode->Key) == Key || (Key != NULL && HashMap->Equals_Key(Key, TempKey, KeySize))))    //如果当前节点的下一个节点的Key与Put的Key一样，则表示找到了相同的Key，此时将e指向该节点
                {
                    break;
                }
                LocatedNode = TempNode;
            }
        }
        if (TempNode != NULL)                                        //如果TempNode不为空，则表示找到了相同的Key，此时替换Value
        {
            void* OldValue = TempNode->Value;
            if (TempNode->Value == NULL) TempNode->Value = Value;    //如果原来的Value为空，则直接赋值
            else                                                     //如果原来的Value不为空，则根据OnlyIfAbsent的值来决定是否替换
            {
                if (!OnlyIfAbsent) TempNode->Value = Value;
            }
            return OldValue;
        }
    }
    if (++HashMap->Size > HashMap->Threshold) { LTT_HashMap_Resize(HashMap); }
    return NULL;
}

void* LTT_HashMap_Put(HashMap* const HashMap, void* const Key, void* const Value) { return LTT_HashMap_PutVal(HashMap, Key, Value, LTT_HashMap_hash(HashMap, Key), false); }

HashNode* LTT_HashMap_GetNode(const HashMap* const HashMap, const void* const Key)
{
    HashNode**  Tab;
    HashNode*   First;
    HashNode*   Temp_HNP;
    int         KeySize = HashMap->KeySize;
    int         Capacity;
    int         hash;
    const void* Temp_Key;
    if ((Tab = HashMap->Table) != NULL && (Capacity = HashMap->Capacity) > 0 && (First = HashMap->Table[(hash = LTT_HashMap_hash(HashMap, Key)) & (Capacity - 1)]) != NULL)
    {
        if (First->hash == hash && ((Temp_Key = First->Key) == Key || (Key != NULL && HashMap->Equals_Key(Key, Temp_Key, KeySize)))) return First;
        if ((Temp_HNP = First->Next) != NULL)
        {
            do {
                if (Temp_HNP->hash == hash && ((Temp_Key = Temp_HNP->Key) == Key || (Key != NULL && HashMap->Equals_Key(Key, Temp_Key, KeySize)))) return Temp_HNP;
            }
            while ((Temp_HNP = Temp_HNP->Next) != NULL);
        }
    }
    return NULL;
}

void* LTT_HashMap_Get(const HashMap* const HashMap, const void* const Key)
{
    HashNode* e = LTT_HashMap_GetNode(HashMap, Key);
    return (e == NULL) ? NULL : e->Value;
}

void* LTT_HashMap_PutIfAbsent(HashMap* const HashMap, void* const Key, void* const Value) { return LTT_HashMap_PutVal(HashMap, Key, Value, LTT_HashMap_hash(HashMap, Key), true); }

bool LTT_HashMap_ContainsKey(const HashMap* const HashMap, const void* const Key) { return LTT_HashMap_GetNode(HashMap, Key) != NULL; }

void* LTT_HashMap_Replace(HashMap* const HashMap, void* const Key, void* const Value)
{
    void* CurrentValue;
    if ((CurrentValue = LTT_HashMap_Get(HashMap, Key)) != NULL || LTT_HashMap_ContainsKey(HashMap, Key)) { CurrentValue = LTT_HashMap_Put(HashMap, Key, Value); }
    return CurrentValue;
}

bool LTT_HashMap_IsEmpty(const HashMap* const HashMap) { return HashMap->Size == 0; }

int LTT_HashMap_GetSize(const HashMap* const HashMap) { return HashMap->Size; }

bool LTT_HashMap_ContainsValue(const HashMap* const HashMap, const void* const Value)
{
    HashNode** Tab;
    void*      Temp_Value;
    if ((Tab = HashMap->Table) != NULL && HashMap->Size > 0)
    {
        for (int i = 0; i < HashMap->Capacity; ++i)
        {
            HashNode* Temp_HNP = Tab[i];
            while (Temp_HNP != NULL)
            {
                if ((Temp_Value = Temp_HNP->Value) == Value || (Value != NULL && HashMap->Equals_Value(Value, Temp_Value, HashMap->ValueSize))) return true;
                Temp_HNP = Temp_HNP->Next;
            }
        }
    }
    return false;
}

HashNode* LTT_HashMap_DeleteNode(HashMap* const HashMap, const void* const Key, const void* const Value, const int hash, const bool matchValue)
{
    HashNode** Tab;
    HashNode*  P;
    int        Capacity;
    int        index;
    int        KeySize   = HashMap->KeySize;
    int        ValueSize = HashMap->ValueSize;
    //P是定位到的头节点
    if ((Tab = HashMap->Table) != NULL && (Capacity = HashMap->Capacity) > 0 && (P = Tab[index = hash & (Capacity - 1)]) != NULL)
    {
        HashNode*   node = NULL;    //用来表示最终定位到的节点
        HashNode*   E;              //P和e用来遍历链表
        const void* Temp_Key;       //用来表示P的Key
        void*       Temp_Value;
        //如果P的hash值与传入的hash值相等，并且P的Key与传入的Key相等，则表示定位到了
        if (P->hash == hash && ((Temp_Key = P->Key) == Key || (Key != NULL && HashMap->Equals_Key(Key, Temp_Key, KeySize)))) { node = P; }
        else if ((E = P->Next) != NULL)    //如果P不是定位到的节点，则遍历链表
        {
            do {
                if (E->hash == hash && ((Temp_Key = E->Key) == Key || (Key != NULL && HashMap->Equals_Key(Key, Temp_Key, KeySize))))
                {
                    node = E;
                    break;
                }
                P = E;
            }
            while ((E = E->Next) != NULL);
        }
        //如果定位到了节点，并且如果matchValue为true，则表示还需要判断Value是否相等
        //如果matchValue为false，则表示不需要判断Value是否相等
        if (node != NULL && (!matchValue || (Temp_Value = node->Value) == Value || (Value != NULL && HashMap->Equals_Value(Value, Temp_Value, ValueSize))))
        {
            if (node == P) Tab[index] = node->Next;
            else P->Next = node->Next;
        }
        --HashMap->Capacity;
        return node;
    }
    return NULL;
}

void* LTT_HashMap_DeleteKey(HashMap* const HashMap, const void* const Key)
{
    HashNode* e;
    return (e = LTT_HashMap_DeleteNode(HashMap, Key, NULL, LTT_HashMap_hash(HashMap, Key), false)) == NULL ? NULL : e->Value;
}

void* LTT_HashMap_DeleteKVPair(HashMap* const HashMap, const void* const Key, const void* const Value) { return LTT_HashMap_DeleteNode(HashMap, Key, Value, LTT_HashMap_hash(HashMap, Key), true); }

void LTT_HashMap_Clear(HashMap* const HashMap)
{
    HashNode** Tab;
    if ((Tab = HashMap->Table) != NULL && HashMap->Size > 0)
    {
        HashMap->Size = 0;
        for (int i = 0; i < HashMap->Capacity; ++i) Tab[i] = NULL;
    }
}

void LTT_HashMap_Destroy(HashMap* HashMap)
{
    free(HashMap->Table);
    free(HashMap);
    HashMap = NULL;
}
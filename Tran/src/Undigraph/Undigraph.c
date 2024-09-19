#include "Undigraph.h"
#include "ArrayQueue.h"
#include "ArrayStack.h"
#include "HashMap.h"
#include "UnionFindSet.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define VID_MAX_LENGTH (1 << 5)    //顶点ID的最大长度
#define EID_MAX_LENGTH (1 << 6)    //边ID的最大长度

//用来存储图的所有节点指针的数组，以方便后面的一些算法的实现
static AMLVertexNode** __VertexArray = NULL;

typedef struct _EdgeNode
{
    VisitIf                Mark;
    struct _AMLVertexNode* IVertex;    //用来标记边的两个顶点
    struct _AMLVertexNode* JVertex;
    char                   EdgeID[EID_MAX_LENGTH];
    struct _EdgeNode*      IEdge;
    struct _EdgeNode*      JEdge;
    void*                  Data;
    size_t                 DataSize;
} _EdgeNode;

typedef struct _AMLVertexNode
{
    VisitIf    Mark;
    char       VertexID[VID_MAX_LENGTH];    //此处用字符串来唯一标识顶点
    void*      Data;
    size_t     DataSize;
    _EdgeNode* FirstEdge;
} _AMLVertexNode;

typedef struct _AMLUndigraph
{
    int      VertexNum;
    int      EdgeNum;
    HashMap* VertexHashMap;
    HashMap* EdgeHashMap;
} _AMLUndigraph;

AMLVertexNode* LTT_Undigraph_GetCurrent_Vertex(const Iterator* const IP) { return (AMLVertexNode*)LTT_HashMap_GetCurrent_Value(IP); }

EdgeNode* LTT_Undigraph_GetCurrent_Edge(const Iterator* const IP) { return (EdgeNode*)LTT_HashMap_GetCurrent_Value(IP); }

Iterator LTT_Undigraph_GetIterator_Vertex(const AMLUndigraph* const GP)
{
    if (GP->VertexNum < 1)
    {
        printf("图的顶点集为空!\n");
        return Iterator_NULL;
    }
    return LTT_HashMap_GetIterator(GP->VertexHashMap);
}

Iterator LTT_Undigraph_GetIterator_Edge(const AMLUndigraph* const GP)
{
    if (GP->VertexNum < 1)
    {
        printf("图的边集为空\n");
        return Iterator_NULL;
    }
    return LTT_HashMap_GetIterator(GP->EdgeHashMap);
}

static EdgeNode** LTT_Undigraph_Get_Tail_EdgePP_Unsafe(const AMLUndigraph* const GP, AMLVertexNode* const VP)
{
    if (VP->FirstEdge == NULL) return &(VP->FirstEdge);
    EdgeNode* Iterator = VP->FirstEdge;
    while (Iterator != NULL)
    {
        if (!strcmp(VP->VertexID, Iterator->IVertex->VertexID))
        {
            if (Iterator->IEdge == NULL) return &(Iterator->IEdge);
            Iterator = Iterator->IEdge;
        }
        else
        {
            if (Iterator->JEdge == NULL) return &(Iterator->JEdge);
            Iterator = Iterator->JEdge;
        }
    }
    return NULL;
}

inline static AMLVertexNode* LTT_Undigraph_Vertex_Exist_byID_Unsafe(const AMLUndigraph* const GP, const char* const VertexID) { return (AMLVertexNode*)LTT_HashMap_Get(GP->VertexHashMap, VertexID); }

inline static void LTT_Undigraph_StandiardizeVertexID(char* const NewVID, const char* const OriginalVID)
{
    int Length = strnlen(OriginalVID, sizeof(char[VID_MAX_LENGTH]));
    memset(NewVID, 0, sizeof(char[VID_MAX_LENGTH]));
    memcpy(NewVID, OriginalVID, Length * sizeof(char));
}

inline static void LTT_Undigraph_StandiardizeEdgeID(char* const NewEID, const char* const IvertexID, const char* const JvertexID)
{
    int IvertexID_Length = strnlen(IvertexID, sizeof(char[VID_MAX_LENGTH]));
    int JvertexID_Length = strnlen(JvertexID, sizeof(char[VID_MAX_LENGTH]));
    memset(NewEID, 0, sizeof(char[EID_MAX_LENGTH]));
    if (strcmp(IvertexID, JvertexID) <= 0)
    {
        memcpy(NewEID, IvertexID, (IvertexID_Length + 1) * sizeof(char));
        memcpy(NewEID + IvertexID_Length + 1, JvertexID, (JvertexID_Length + 1) * sizeof(char));
    }
    else
    {
        memcpy(NewEID, JvertexID, (JvertexID_Length + 1) * sizeof(char));
        memcpy(NewEID + JvertexID_Length + 1, IvertexID, (IvertexID_Length + 1) * sizeof(char));
    }
}

AMLVertexNode* LTT_Undigraph_Vertex_Exist_byID_Safe(const AMLUndigraph* const GP, const char* const VertexID)
{
    char NewVID[VID_MAX_LENGTH];
    LTT_Undigraph_StandiardizeVertexID(NewVID, VertexID);
    return (AMLVertexNode*)LTT_HashMap_Get(GP->VertexHashMap, NewVID);
}

bool LTT_Undigraph_Edge_Exist_by_EP(const AMLUndigraph* const GP, const EdgeNode* const EP)
{
    if (EP == NULL) return false;
    EdgeNode* EP_In_Graph = LTT_HashMap_Get(GP->EdgeHashMap, EP->EdgeID);
    if (EP_In_Graph == NULL || EP_In_Graph != EP) return false;
    else return true;
}

int LTT_Undigraph_Get_VertexNum(const AMLUndigraph* const GP) { return GP->VertexNum; }

int LTT_Undigraph_Get_EdgeNum(const AMLUndigraph* const GP) { return GP->EdgeNum; }

Status LTT_Undigraph_Insert_Edge(AMLUndigraph* const GP, EdgeNode* const EP)
{
    if (strcmp(EP->IVertex->VertexID, EP->JVertex->VertexID) == 0)
    {
        printf("插入的边非法\n");
        return ERROR;
    }
    if (LTT_Undigraph_Edge_Exist_by_EP(GP, EP) == true)
    {
        printf("边已存在\n");
        return ERROR;
    }

    AMLVertexNode* IP_In_Graph = EP->IVertex;    //LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, EP->IvertexID);
    AMLVertexNode* JP_In_Graph = EP->JVertex;    //LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, EP->JvertexID);
    EdgeNode**     ITailEdgePP = LTT_Undigraph_Get_Tail_EdgePP_Unsafe(GP, IP_In_Graph);
    EdgeNode**     JTailEdgePP = LTT_Undigraph_Get_Tail_EdgePP_Unsafe(GP, JP_In_Graph);
    if (ITailEdgePP == NULL || JTailEdgePP == NULL)
    {
        printf("图的构建出错\n");
        return ERROR;
    }
    ++GP->EdgeNum;
    //注意，这里指向的指针应该是Hash表中的指针，而不是外部传入的指针
    LTT_HashMap_Put(GP->EdgeHashMap, EP->EdgeID, EP);
    *ITailEdgePP = *JTailEdgePP = LTT_HashMap_Get(GP->EdgeHashMap, EP->EdgeID);
    return OK;
}

Status LTT_Undigraph_Insert_EdgeS(AMLUndigraph* const GP, EdgeNode** const EdgeArray, const int EdgeNum)
{
    for (int i = 0; i < EdgeNum; ++i)
    {
        if (LTT_Undigraph_Insert_Edge(GP, EdgeArray[i]) == ERROR)
        {
            printf("图在构建时插入边失败\n");
            return ERROR;
        }
    }
    return OK;
}

bool LTT_Undigraph_Vertex_Exist_byVP(const AMLUndigraph* const GP, const AMLVertexNode* const VP)
{
    if (VP == NULL) return false;
    AMLVertexNode* VP_In_Graph = LTT_HashMap_Get(GP->VertexHashMap, VP->VertexID);
    if (VP_In_Graph == NULL || VP_In_Graph != VP) return false;
    else return true;
}

Status LTT_Undigraph_Insert_Vertex(AMLUndigraph* const GP, AMLVertexNode* const VP)
{
    if (VP == NULL)
    {
        printf("顶点未声明\n");
        return ERROR;
    }
    if (LTT_Undigraph_Vertex_Exist_byVP(GP, VP) == true)
    {
        printf("顶点已存在\n");
        return ERROR;
    }
    ++GP->VertexNum;
    LTT_HashMap_Put(GP->VertexHashMap, VP->VertexID, VP);
    return OK;
}

Status LTT_Undigraph_Insert_VertexS(AMLUndigraph* const GP, AMLVertexNode** const AMLVertexArray, const int VertexNum)
{
    for (int i = 0; i < VertexNum; ++i)
    {
        if (LTT_Undigraph_Insert_Vertex(GP, AMLVertexArray[i]) == ERROR)
        {
            printf("图在构建时插入点失败\n");
            return ERROR;
        }
    }
    return OK;
}

AMLUndigraph* LTT_Undigraph_New(void)
{
    AMLUndigraph* GP = (AMLUndigraph*)malloc(sizeof(AMLUndigraph));
    if (GP == NULL)
    {
        printf("图未声明\n");
        return NULL;
    }
    GP->VertexNum     = 0;
    GP->EdgeNum       = 0;
    GP->VertexHashMap = LTT_HashMap_New(VID_MAX_LENGTH * sizeof(char), sizeof(AMLVertexNode), NULL, NULL, NULL);
    GP->EdgeHashMap   = LTT_HashMap_New(EID_MAX_LENGTH * sizeof(char), sizeof(EdgeNode), NULL, NULL, NULL);
    return GP;
}

void LTT_Undigraph_Clear(AMLUndigraph* const GP)
{
    LTT_HashMap_Clear(GP->EdgeHashMap);
    LTT_HashMap_Clear(GP->VertexHashMap);
    GP->EdgeNum   = 0;
    GP->VertexNum = 0;
}

void LTT_Undigraph_Destroy(AMLUndigraph** GPP)
{
    LTT_HashMap_Destroy(&(*GPP)->EdgeHashMap);
    LTT_HashMap_Destroy(&(*GPP)->VertexHashMap);
    *GPP = NULL;
}

AMLVertexNode* LTT_Undigraph_Make_Vertex(const char* const VertexID, void* const Data, const size_t DataSize)
{
    AMLVertexNode* VP = (AMLVertexNode*)malloc(sizeof(AMLVertexNode));
    if (VP == NULL)
    {
        printf("顶点未声明\n");
        return NULL;
    }
    VP->Data = Data;
    LTT_Undigraph_StandiardizeVertexID(VP->VertexID, VertexID);
    VP->DataSize  = DataSize;
    VP->FirstEdge = NULL;
    VP->Mark      = Unvisited;
    return VP;
}

static EdgeNode** LTT_Undigraph_Get_Prior_EdgePP_Unsafe(const AMLUndigraph* const GP, AMLVertexNode* const VP, const EdgeNode* const EP)
{
    if (VP->FirstEdge == EP) return &(VP->FirstEdge);
    EdgeNode* Iterator = VP->FirstEdge;
    while (Iterator)
    {
        if (strcmp(VP->VertexID, Iterator->IVertex->VertexID) == 0)
        {
            if (EP == Iterator->IEdge) return &(Iterator->IEdge);
            else Iterator = Iterator->IEdge;
        }
        else
        {
            if (EP == Iterator->JEdge) return &(Iterator->JEdge);
            else Iterator = Iterator->JEdge;
        }
    }
    return NULL;
}

static Status LTT_Undigraph_Delete_Edge_byEP_Unsafe(AMLUndigraph* const GP, EdgeNode* EP)
{
    AMLVertexNode* IP_In_Graph  = EP->IVertex;    //LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, EP->IvertexID);
    AMLVertexNode* JP_In_Graph  = EP->JVertex;    //LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, EP->JvertexID);
    EdgeNode**     IPriorEdgePP = LTT_Undigraph_Get_Prior_EdgePP_Unsafe(GP, IP_In_Graph, EP);
    EdgeNode**     JPriorEdgePP = LTT_Undigraph_Get_Prior_EdgePP_Unsafe(GP, JP_In_Graph, EP);
    (*IPriorEdgePP)             = EP->IEdge;
    (*JPriorEdgePP)             = EP->JEdge;
    EP->Data                    = NULL;
    LTT_HashMap_DeleteKey(GP->EdgeHashMap, EP->EdgeID);
    --GP->EdgeNum;
    return OK;
}

Status LTT_Undigraph_DeleteVertex_byID(AMLUndigraph* const GP, const char* const VertexID)
{
    char NewVID[VID_MAX_LENGTH];
    LTT_Undigraph_StandiardizeVertexID(NewVID, VertexID);
    AMLVertexNode* VP_In_Graph = LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, NewVID);
    if (VP_In_Graph == NULL)
    {
        printf("顶点不存在\n");
        return ERROR;
    }
    --GP->VertexNum;
    if (VP_In_Graph->FirstEdge == NULL)
    {
        VP_In_Graph->Data = NULL;
        LTT_HashMap_DeleteKey(GP->VertexHashMap, NewVID);
        return OK;
    }
    EdgeNode* Iterator = VP_In_Graph->FirstEdge;
    while (Iterator)
    {
        EdgeNode* Temp = Iterator;
        if (strcmp(VP_In_Graph->VertexID, Iterator->IVertex->VertexID) == 0) Iterator = Iterator->IEdge;
        else Iterator = Iterator->JEdge;
        LTT_Undigraph_Delete_Edge_byEP_Unsafe(GP, Temp);
    }
    VP_In_Graph->Data = NULL;
    LTT_HashMap_DeleteKey(GP->VertexHashMap, NewVID);
    return OK;
}

Status LTT_Undigraph_Delete_Vertex_byVP(AMLUndigraph* const GP, AMLVertexNode* VP)
{
    if (LTT_Undigraph_Vertex_Exist_byVP(GP, VP) == false)
    {
        printf("顶点不存在\n");
        return ERROR;
    }
    --GP->VertexNum;
    if (VP->FirstEdge == NULL)
    {
        VP->Data = NULL;
        LTT_HashMap_DeleteKey(GP->VertexHashMap, VP->VertexID);
        return OK;
    }
    EdgeNode* Iterator = VP->FirstEdge;
    while (Iterator)
    {
        EdgeNode* Temp = Iterator;
        if (strcmp(VP->VertexID, Iterator->IVertex->VertexID) == 0) Iterator = Iterator->IEdge;
        else Iterator = Iterator->JEdge;
        LTT_Undigraph_Delete_Edge_byEP_Unsafe(GP, Temp);
    }
    VP->Data = NULL;
    LTT_HashMap_DeleteKey(GP->VertexHashMap, VP->VertexID);
    return OK;
}

void* LTT_Undigraph_Get_Data_From_Vertex_byID(const AMLUndigraph* const GP, const char* const VertexID)
{
    char NewVID[VID_MAX_LENGTH];
    LTT_Undigraph_StandiardizeVertexID(NewVID, VertexID);
    return LTT_HashMap_Get(GP->VertexHashMap, NewVID);
}

void* LTT_Undigraph_Get_Data_From_Vertex_byVP(const AMLUndigraph* const GP, const AMLVertexNode* const VP)
{
    if (LTT_Undigraph_Vertex_Exist_byVP(GP, VP) == false)
    {
        printf("该顶点不在图内\n");
        return NULL;
    }
    return VP->Data;
}

void* LTT_Undigraph_Get_Data_From_Vertex_byVP_Uncheck(const AMLVertexNode* const VP) { return VP->Data; }

const char* LTT_Undigraph_Get_VertexID_byVP_Uncheck(const AMLVertexNode* const VP) { return VP->VertexID; }

Status LTT_Undigraph_Set_Data_From_Vertex_byID(const AMLUndigraph* const GP, const char* const VertexID, const void* const Data, const size_t DataSize)
{
    char NewVID[VID_MAX_LENGTH];
    LTT_Undigraph_StandiardizeVertexID(NewVID, VertexID);
    if (LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, NewVID) == false)
    {
        printf("顶点不存在\n");
        return ERROR;
    }
    AMLVertexNode* VP_In_Graph = LTT_HashMap_Get(GP->VertexHashMap, NewVID);
    VP_In_Graph->DataSize      = DataSize;
    memcpy(VP_In_Graph->Data, Data, DataSize);
    return OK;
}

Status LTT_Undigraph_Set_Data_From_Vertex_byVP(const AMLUndigraph* const GP, AMLVertexNode* const VP, const void* const Data, const size_t DataSize)
{
    if (LTT_Undigraph_Vertex_Exist_byVP(GP, VP) == false)
    {
        printf("该顶点不在图内\n");
        return ERROR;
    }
    memcpy(VP->Data, Data, DataSize);
    return OK;
}

static EdgeNode* LTT_Undigraph_Make_Edge_byVP_Unsafe(AMLVertexNode* const VIP, AMLVertexNode* const VJP, void* const Data, const size_t DataSize)
{
    EdgeNode* EP = (EdgeNode*)malloc(sizeof(EdgeNode));
    if (EP == NULL)
    {
        printf("边未声明\n");
        return NULL;
    }
    EP->Data     = Data;
    EP->IEdge    = NULL;
    EP->JEdge    = NULL;
    EP->Mark     = Unvisited;
    EP->DataSize = DataSize;
    EP->IVertex  = VIP;
    EP->JVertex  = VJP;
    LTT_Undigraph_StandiardizeEdgeID(EP->EdgeID, VIP->VertexID, VJP->VertexID);
    return EP;
}

EdgeNode* LTT_Undigraph_Make_Edge_byVP_Safe(const AMLUndigraph* const GP, AMLVertexNode* const VIP, AMLVertexNode* const VJP, void* const Data, const size_t DataSize)
{
    if (!LTT_Undigraph_Vertex_Exist_byVP(GP, VIP) || !LTT_Undigraph_Vertex_Exist_byVP(GP, VJP))
    {
        printf("边的顶点不存在\n");
        return NULL;
    }
    return LTT_Undigraph_Make_Edge_byVP_Unsafe(VIP, VJP, Data, DataSize);
}

EdgeNode* LTT_Undigraph_Make_Edge_byID(const AMLUndigraph* const GP, const char* const IvertexID, const char* const JvertexID, void* const Data, const size_t DataSize)
{
    EdgeNode* EP = (EdgeNode*)malloc(sizeof(EdgeNode));
    if (EP == NULL)
    {
        printf("边未声明\n");
        return NULL;
    }
    EP->Data     = Data;
    EP->IEdge    = NULL;
    EP->JEdge    = NULL;
    EP->Mark     = Unvisited;
    EP->DataSize = DataSize;
    EP->IVertex  = LTT_Undigraph_Vertex_Exist_byID_Safe(GP, IvertexID);
    EP->JVertex  = LTT_Undigraph_Vertex_Exist_byID_Safe(GP, JvertexID);
    if (EP->IVertex == NULL || EP->JVertex == NULL)
    {
        printf("边的顶点不存在\n");
        return NULL;
    }
    LTT_Undigraph_StandiardizeEdgeID(EP->EdgeID, IvertexID, JvertexID);
    return EP;
}

EdgeNode* LTT_Undigraph_Edge_Exist_by_ID(const AMLUndigraph* const GP, const char* const IvertexID, const char* const JvertexID)
{
    char NewEID[EID_MAX_LENGTH];
    LTT_Undigraph_StandiardizeEdgeID(NewEID, IvertexID, JvertexID);
    return (EdgeNode*)LTT_HashMap_Get(GP->EdgeHashMap, NewEID);
}

Status LTT_Undigraph_Delete_Edge_byID(AMLUndigraph* const GP, const char* const IvertexID, const char* const JvertexID)
{
    char NewIvertexID[VID_MAX_LENGTH];
    char NewJvertexID[VID_MAX_LENGTH];
    LTT_Undigraph_StandiardizeVertexID(NewIvertexID, IvertexID);
    LTT_Undigraph_StandiardizeVertexID(NewJvertexID, JvertexID);
    EdgeNode* EP_In_Graph = LTT_Undigraph_Edge_Exist_by_ID(GP, IvertexID, JvertexID);
    if (EP_In_Graph == NULL)
    {
        printf("边不存在\n");
        return ERROR;
    }
    AMLVertexNode* IP_In_Graph  = LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, NewIvertexID);
    AMLVertexNode* JP_In_Graph  = LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, NewJvertexID);
    EdgeNode**     IPriorEdgePP = LTT_Undigraph_Get_Prior_EdgePP_Unsafe(GP, IP_In_Graph, EP_In_Graph);
    EdgeNode**     JPriorEdgePP = LTT_Undigraph_Get_Prior_EdgePP_Unsafe(GP, JP_In_Graph, EP_In_Graph);
    if (strcmp(IvertexID, EP_In_Graph->IVertex->VertexID) == 0)
    {
        (*IPriorEdgePP) = EP_In_Graph->IEdge;
        (*JPriorEdgePP) = EP_In_Graph->JEdge;
    }
    else
    {
        (*IPriorEdgePP) = EP_In_Graph->JEdge;
        (*JPriorEdgePP) = EP_In_Graph->IEdge;
    }
    EP_In_Graph->Data = NULL;
    LTT_HashMap_DeleteKey(GP->EdgeHashMap, EP_In_Graph->EdgeID);
    --GP->EdgeNum;
    return OK;
}

Status LTT_Undigraph_Delete_Edge_byEP_Safe(AMLUndigraph* const GP, EdgeNode* EP)
{
    if (LTT_Undigraph_Edge_Exist_by_EP(GP, EP) == false)
    {
        printf("边不存在\n");
        return ERROR;
    }
    LTT_Undigraph_Delete_Edge_byEP_Unsafe(GP, EP);
    return OK;
}

void* LTT_Undigraph_Get_Data_From_Edge_byID(const AMLUndigraph* const GP, const char* IvertexID, const char* JvertexID)
{
    EdgeNode* EP_In_Graph = LTT_Undigraph_Edge_Exist_by_ID(GP, IvertexID, JvertexID);
    if (EP_In_Graph == NULL)
    {
        printf("边不存在\n");
        return NULL;
    }
    return EP_In_Graph->Data;
}

void* LTT_Undigraph_Get_Data_From_Edge_byEP(const AMLUndigraph* const GP, const EdgeNode* const EP)
{
    if (LTT_Undigraph_Edge_Exist_by_EP(GP, EP) == false)
    {
        printf("边不存在\n");
        return NULL;
    }
    return EP->Data;
}

void* LTT_Undigraph_Get_Data_From_Edge_byEP_Uncheck(const EdgeNode* const EP) { return EP->Data; }

Status LTT_Undigraph_Set_Data_From_Edge_byID(const AMLUndigraph* const GP, const char* const IvertexID, const char* const JvertexID, const void* Data, const size_t DataSize)
{
    EdgeNode* EP_In_Graph = LTT_Undigraph_Edge_Exist_by_ID(GP, IvertexID, JvertexID);
    if (EP_In_Graph == NULL)
    {
        printf("边不存在\n");
        return ERROR;
    }
    memcpy(EP_In_Graph->Data, Data, DataSize);
    return OK;
}

Status LTT_Undigraph_Set_Data_From_Edge_byEP(const AMLUndigraph* const GP, EdgeNode* const EP, const void* Data, const size_t DataSize)
{
    if (LTT_Undigraph_Edge_Exist_by_EP(GP, EP) == false)
    {
        printf("边不存在\n");
        return ERROR;
    }
    memcpy(EP->Data, Data, DataSize);
    return OK;
}

static Status LTT_Undigraph_Make_VertexArray(const AMLUndigraph* const GP)
{
    if (__VertexArray != NULL) free(__VertexArray);
    __VertexArray = (AMLVertexNode**)malloc((GP->VertexNum + 1) * sizeof(AMLVertexNode*));
    if (__VertexArray == NULL)
    {
        printf("数组内存分配失败\n");
        return OVERFLOW;
    }
    Iterator VertexIterator = LTT_Undigraph_GetIterator_Vertex(GP);
    for (int i = 1; VertexIterator.MoveNext(&VertexIterator); ++i) { __VertexArray[i] = LTT_Undigraph_GetCurrent_Vertex(&VertexIterator); }
    return OK;
}

int** LTT_Undigraph_Make_AdjMatrix(const AMLUndigraph* const GP)
{
    if (GP == NULL)
    {
        printf("图未声明\n");
        return NULL;
    }
    //初始化邻接矩阵,一共有VertexNum+1行,多的一行用来存储每个顶点的度
    int** AdjMatrix = (int**)malloc((GP->VertexNum + 1) * sizeof(int*));
    if (AdjMatrix == NULL)
    {
        printf("二维数组内存分配失败!\n");
        return NULL;
    }
    for (int i = 0; i <= GP->VertexNum; ++i)
    {
        if (!(AdjMatrix[i] = (int*)calloc(GP->VertexNum + 1, sizeof(int))))
        {
            printf("邻接矩阵内存分配失败\n");
            return NULL;
        }
    }
    //先将__VertexArray填满
    LTT_Undigraph_Make_VertexArray(GP);

    for (int i = 1; i <= GP->VertexNum; ++i)
    {
        for (int j = i + 1; j <= GP->VertexNum; ++j)
        {
            EdgeNode* EdgeP = LTT_Undigraph_Edge_Exist_by_ID(GP, __VertexArray[i]->VertexID, __VertexArray[j]->VertexID);
            if (EdgeP != NULL)
            {
                AdjMatrix[i][j] = AdjMatrix[j][i] = 1;
                ++AdjMatrix[i][0];
                ++AdjMatrix[j][0];
                ++AdjMatrix[0][i];
                ++AdjMatrix[0][j];
            }
        }
    }
    return AdjMatrix;
}

int** LTT_Undigraph_Make_Weighted_AdjMatrix(const AMLUndigraph* const GP)
{
    if (GP == NULL)
    {
        printf("图未声明\n");
        return NULL;
    }
    //初始化邻接矩阵,一共有VertexNum+1行,多的一行用来存储每个顶点的度
    int** AdjMatrix = (int**)malloc((GP->VertexNum + 1) * sizeof(int*));
    if (AdjMatrix == NULL)
    {
        printf("二维数组内存分配失败!\n");
        return NULL;
    }
    for (int i = 0; i <= GP->VertexNum; ++i)
    {
        if (!(AdjMatrix[i] = (int*)calloc(GP->VertexNum + 1, sizeof(int))))
        {
            printf("邻接矩阵内存分配失败\n");
            return NULL;
        }
    }
    //先将__VertexArray填满
    LTT_Undigraph_Make_VertexArray(GP);

    for (int i = 1; i <= GP->VertexNum; ++i)
    {
        for (int j = i + 1; j <= GP->VertexNum; ++j)
        {
            EdgeNode* EdgeP = LTT_Undigraph_Edge_Exist_by_ID(GP, __VertexArray[i]->VertexID, __VertexArray[j]->VertexID);
            if (EdgeP != NULL)
            {
                AdjMatrix[i][j] = AdjMatrix[j][i] = *(int*)EdgeP->Data;
                ++AdjMatrix[i][0];
                ++AdjMatrix[j][0];
                ++AdjMatrix[0][i];
                ++AdjMatrix[0][j];
            }
            else { AdjMatrix[i][j] = AdjMatrix[j][i] = INT_MAX; }
        }
    }
    return AdjMatrix;
}

AMLVertexNode* LTT_Undigraph_First_Adj_Vertex_byID(const AMLUndigraph* const GP, const char* const VertexID)
{
    AMLVertexNode* VP_In_Graph = LTT_Undigraph_Vertex_Exist_byID_Safe(GP, VertexID);
    if (VP_In_Graph == NULL)
    {
        printf("顶点不存在\n");
        return NULL;
    }
    if (VP_In_Graph->FirstEdge == NULL)
    {
        printf("顶点无邻接点\n");
        return NULL;
    }
    if (strcmp(VertexID, VP_In_Graph->FirstEdge->IVertex->VertexID) == 0) return LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, VP_In_Graph->FirstEdge->JVertex->VertexID);
    else return LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, VP_In_Graph->FirstEdge->IVertex->VertexID);
}

AMLVertexNode* LTT_Undigraph_First_AML_Vertex_byVP(const AMLUndigraph* const GP, const AMLVertexNode* const VP)
{
    if (LTT_Undigraph_Vertex_Exist_byVP(GP, VP) == false)
    {
        printf("顶点不存在\n");
        return NULL;
    }
    if (VP->FirstEdge == NULL)
    {
        printf("顶点无邻接点\n");
        return NULL;
    }
    else
    {
        if (strcmp(VP->VertexID, VP->FirstEdge->IVertex->VertexID) == 0) return LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, VP->FirstEdge->JVertex->VertexID);
        else return LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, VP->FirstEdge->IVertex->VertexID);
    }
}

AMLVertexNode* LTT_Undigraph_Frist_Unvisited_Adj_Vertex_byID(const AMLUndigraph* const GP, const char* const VertexID)
{
    AMLVertexNode* VP_In_Graph = LTT_Undigraph_Vertex_Exist_byID_Safe(GP, VertexID);
    if (VP_In_Graph == NULL)
    {
        printf("顶点不存在\n");
        return NULL;
    }
    if (VP_In_Graph->FirstEdge == NULL)
    {
        printf("顶点无邻接点\n");
        return NULL;
    }
    EdgeNode* Iterator = VP_In_Graph->FirstEdge;
    while (Iterator)
    {
        if (strcmp(VP_In_Graph->VertexID, Iterator->IVertex->VertexID) == 0)
        {
            AMLVertexNode* Temp_J = LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, Iterator->JVertex->VertexID);
            if (Temp_J->Mark == Unvisited) return Temp_J;
            else Iterator = Iterator->IEdge;
        }
        else
        {
            AMLVertexNode* Temp_I = LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, Iterator->IVertex->VertexID);
            if (Temp_I->Mark == Unvisited) return Temp_I;
            else Iterator = Iterator->JEdge;
        }
    }
    return NULL;
}

static AMLVertexNode* LTT_Undigraph_Frist_Unvisited_Adj_Vertex_byVP_Unsafe(const AMLUndigraph* const GP, const AMLVertexNode* const VP)
{
    if (VP->FirstEdge == NULL) return NULL;
    EdgeNode* Iterator = VP->FirstEdge;
    while (Iterator)
    {
        if (strcmp(VP->VertexID, Iterator->IVertex->VertexID) == 0)
        {
            AMLVertexNode* Temp_J = LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, Iterator->JVertex->VertexID);
            if (Temp_J->Mark == Unvisited) return Temp_J;
            else Iterator = Iterator->IEdge;
        }
        else
        {
            AMLVertexNode* Temp_I = LTT_Undigraph_Vertex_Exist_byID_Unsafe(GP, Iterator->IVertex->VertexID);
            if (Temp_I->Mark == Unvisited) return Temp_I;
            else Iterator = Iterator->JEdge;
        }
    }
    return NULL;
}

AMLVertexNode* LTT_Undigraph_Frist_Unvisited_Adj_Vertex_byVP_Safe(const AMLUndigraph* const GP, const AMLVertexNode* const VP)
{
    if (LTT_Undigraph_Vertex_Exist_byVP(GP, VP) == false)
    {
        printf("顶点不存在\n");
        return NULL;
    }
    if (VP->FirstEdge == NULL)
    {
        printf("顶点无邻接点\n");
        return NULL;
    }
    return LTT_Undigraph_Frist_Unvisited_Adj_Vertex_byVP_Unsafe(GP, VP);
}

EdgeNode** LTT_Undigraph_Get_Prior_EdgePP_byID_Safe(const AMLUndigraph* const GP, const char* const VertexID, const char* const IvertexID, const char* const JvertexID)
{
    AMLVertexNode* VP_In_Graph = LTT_Undigraph_Vertex_Exist_byID_Safe(GP, VertexID);
    if (VP_In_Graph == NULL)
    {
        printf("顶点不存在\n");
        return NULL;
    }
    if (VP_In_Graph->FirstEdge == NULL)
    {
        printf("顶点无邻接点\n");
        return NULL;
    }
    EdgeNode* EP_In_Graph = LTT_Undigraph_Edge_Exist_by_ID(GP, IvertexID, JvertexID);
    if (EP_In_Graph == NULL)
    {
        printf("边不存在\n");
        return ERROR;
    }
    return LTT_Undigraph_Get_Prior_EdgePP_Unsafe(GP, VP_In_Graph, EP_In_Graph);
}

EdgeNode** LTT_Undigraph_Get_Prior_EdgePP_byVE_Safe(const AMLUndigraph* const GP, AMLVertexNode* const VP, const EdgeNode* const EP)
{
    if (LTT_Undigraph_Vertex_Exist_byVP(GP, VP) == false)
    {
        printf("顶点不存在\n");
        return NULL;
    }
    if (LTT_Undigraph_Edge_Exist_by_EP(GP, EP) == false)
    {
        printf("边不存在\n");
        return NULL;
    }
    return LTT_Undigraph_Get_Prior_EdgePP_Unsafe(GP, VP, EP);
}

EdgeNode** LTT_Undigraph_Get_Tail_EdgePP_by_ID_Safe(const AMLUndigraph* const GP, const char* const VertexID)
{
    AMLVertexNode* VP_In_Graph = LTT_Undigraph_First_Adj_Vertex_byID(GP, VertexID);
    if (VP_In_Graph == NULL)
    {
        printf("顶点不存在\n");
        return NULL;
    }
    return LTT_Undigraph_Get_Tail_EdgePP_Unsafe(GP, VP_In_Graph);
}

EdgeNode** LTT_Undigraph_Get_Tail_EdgePP_by_VP_Safe(const AMLUndigraph* const GP, AMLVertexNode* const VP)
{
    if (LTT_Undigraph_Vertex_Exist_byVP(GP, VP) == false)
    {
        printf("顶点不存在\n");
        return NULL;
    }
    return LTT_Undigraph_Get_Tail_EdgePP_Unsafe(GP, VP);
}

static void LTT_Undigraph_DFS_Traverse_Recursive_Loop(const AMLUndigraph* const GP, AMLVertexNode* VP, const VisitFunction Visit)
{
    VP->Mark = Visited;
    if (Visit != NULL) (*Visit)(VP);
    while ((VP = LTT_Undigraph_Frist_Unvisited_Adj_Vertex_byVP_Unsafe(GP, VP)) != NULL) { LTT_Undigraph_DFS_Traverse_Recursive_Loop(GP, VP, Visit); }
}

Status LTT_Undigraph_DFS_Traverse_Recursive(const AMLUndigraph* const GP, const char* const VertexID, const VisitFunction Visit)
{
    if (GP == NULL)
    {
        printf("图未声明\n");
        return ERROR;
    }
    AMLVertexNode* VP_In_Graph = LTT_Undigraph_Vertex_Exist_byID_Safe(GP, VertexID);
    if (VP_In_Graph == NULL)
    {
        printf("顶点不存在\n");
        return ERROR;
    }
    //初始化所有点的可访问性
    Iterator Iterator = LTT_Undigraph_GetIterator_Vertex(GP);
    while (Iterator.MoveNext(&Iterator)) LTT_Undigraph_GetCurrent_Vertex(&Iterator)->Mark = Unvisited;
    // 从VertexID点出发递归调用；
    LTT_Undigraph_DFS_Traverse_Recursive_Loop(GP, VP_In_Graph, Visit);
    return OK;
}

Status LTT_Undigraph_DFS_Traverse_Stack(const AMLUndigraph* const GP, const char* const VertexID, const VisitFunction Visit)
{
    if (GP == NULL)
    {
        printf("图未声明\n");
        return ERROR;
    }
    AMLVertexNode* VP_In_Graph = LTT_Undigraph_Vertex_Exist_byID_Safe(GP, VertexID);
    if (VP_In_Graph == NULL)
    {
        printf("顶点不存在\n");
        return ERROR;
    }
    //初始化所有的点的可访问性
    Iterator Iterator = LTT_Undigraph_GetIterator_Vertex(GP);
    while (Iterator.MoveNext(&Iterator)) LTT_Undigraph_GetCurrent_Vertex(&Iterator)->Mark = Unvisited;
    ArrayStack* Stack = LTT_ArrayStack_New(sizeof(AMLVertexNode*), NULL);
    LTT_ArrayStack_Push(Stack, VP_In_Graph);
    while (!LTT_ArrayStack_IsEmpty(Stack))
    {
        AMLVertexNode* Temp;
        Temp = LTT_ArrayStack_Pop(Stack);
        (*Visit)(Temp);
        Temp->Mark = Visited;
        AMLVertexNode* NextVP;
        while ((NextVP = LTT_Undigraph_Frist_Unvisited_Adj_Vertex_byVP_Unsafe(GP, Temp)) != NULL)
        {
            LTT_ArrayStack_Push(Stack, NextVP);
            NextVP->Mark = Visited;
        }
    }
    return OK;
}

Status LTT_Undigraph_BFS_Traverse(const AMLUndigraph* const GP, const char* const VertexID, const VisitFunction Visit)
{
    if (GP == NULL)
    {
        printf("图未声明\n");
        return ERROR;
    }
    AMLVertexNode* VP_In_Graph = LTT_Undigraph_Vertex_Exist_byID_Safe(GP, VertexID);
    if (VP_In_Graph == NULL)
    {
        printf("顶点不存在\n");
        return ERROR;
    }
    //初始化所有的点的可访问性
    Iterator Iterator = LTT_Undigraph_GetIterator_Vertex(GP);
    while (Iterator.MoveNext(&Iterator)) LTT_Undigraph_GetCurrent_Vertex(&Iterator)->Mark = Unvisited;
    ArrayQueue* Queue = LTT_ArrayQueue_New(sizeof(AMLVertexNode*), NULL);
    LTT_ArrayQueue_Push(Queue, VP_In_Graph);
    while (!LTT_ArrayQueue_IsEmpty(Queue))
    {
        AMLVertexNode* Temp;
        Temp = LTT_ArrayQueue_Pop(Queue);
        (*Visit)(Temp);
        Temp->Mark = Visited;
        AMLVertexNode* NextVP;
        while ((NextVP = LTT_Undigraph_Frist_Unvisited_Adj_Vertex_byVP_Unsafe(GP, Temp)) != NULL)
        {
            LTT_ArrayQueue_Push(Queue, NextVP);
            NextVP->Mark = Visited;
        }
    }
    return OK;
}

int LTT_Undigraph_Get_ConnectedComponentNumber(const AMLUndigraph* const GP)
{
    if (GP == NULL)
    {
        printf("图未声明\n");
        return ERROR;
    }
    //初始化所有的点的可访问性
    Iterator Iterator = LTT_Undigraph_GetIterator_Vertex(GP);
    while (Iterator.MoveNext(&Iterator)) LTT_Undigraph_GetCurrent_Vertex(&Iterator)->Mark = Unvisited;
    int count = 0;
    //重置迭代器
    Iterator.Reset(&Iterator);
    while (Iterator.MoveNext(&Iterator))
    {
        AMLVertexNode* VP = LTT_Undigraph_GetCurrent_Vertex(&Iterator);
        if (VP->Mark == Unvisited)
        {
            LTT_Undigraph_DFS_Traverse_Recursive_Loop(GP, VP, NULL);
            count++;
        }
    }
    return count;
}

bool LTT_Undigraph_HasCircle(const AMLUndigraph* const GP) { return (GP->EdgeNum > GP->VertexNum - LTT_Undigraph_Get_ConnectedComponentNumber(GP)) ? true : false; }

AMLUndigraph* LTT_Undigraph_MiniSpanTree_Prim(const AMLUndigraph* const GP, const AMLVertexNode* const VP)
{
    if (LTT_Undigraph_Vertex_Exist_byVP(GP, VP) == false)
    {
        printf("顶点不存在\n");
        return NULL;
    }


    //判断是否联通
    if (LTT_Undigraph_Get_ConnectedComponentNumber(GP) > 1)
    {
        printf("图不连通\n");
        return NULL;
    }

    //在Prim算法中，每一个尚未归入到最小生成树的顶点
    //都对应一个VDPair结构体
    //这个结构体里包含此点到子最小生成树的距离，以及所对应的顶点的指针
    typedef struct
    {
        const AMLVertexNode* ConnectedP;
        int                  Distance;
    } VDPair;

    //用来存储最小生成树的边和顶点
    EdgeNode**      MiTreeEdgeArray   = (EdgeNode**)malloc((GP->VertexNum - 1) * sizeof(EdgeNode*));
    AMLVertexNode** MiTreeVertexArray = (AMLVertexNode**)malloc((GP->VertexNum) * sizeof(AMLVertexNode*));


    //用来临时存储顶点和边
    AMLVertexNode* Temp_VP            = NULL;
    EdgeNode*      Temp_EP            = NULL;
    VDPair*        Temp_VDPP          = NULL;

    //把图的顶点复制到MiTreeVertexArray
    Iterator VertexIterator           = LTT_Undigraph_GetIterator_Vertex(GP);
    int      Temp_i                   = 0;
    while (VertexIterator.MoveNext(&VertexIterator))
    {
        Temp_VP                     = LTT_Undigraph_GetCurrent_Vertex(&VertexIterator);
        MiTreeVertexArray[Temp_i++] = LTT_Undigraph_Make_Vertex(Temp_VP->VertexID, Temp_VP->Data, Temp_VP->DataSize);
    }


    //用一个Hash表，键为顶点指针，值VDPair结构体
    HashMap* CloseEdge = LTT_HashMap_New(sizeof(AMLVertexNode), sizeof(VDPair), NULL, NULL, NULL);
    //初始化CloseEdge，记录每一个顶点到VP的距离
    VertexIterator.Reset(&VertexIterator);    //重置顶点迭代器
    while (VertexIterator.MoveNext(&VertexIterator))
    {
        Temp_VP = LTT_Undigraph_GetCurrent_Vertex(&VertexIterator);
        //获得VP与当前迭代器所对应的边
        Temp_EP = LTT_Undigraph_Edge_Exist_by_ID(GP, VP->VertexID, Temp_VP->VertexID);
        //如果边存在，就把边的权值放入CloseEdge
        if (Temp_EP != NULL)
        {
            Temp_VDPP             = (VDPair*)malloc(sizeof(VDPair));
            Temp_VDPP->ConnectedP = VP;
            Temp_VDPP->Distance   = *(int*)Temp_EP->Data;
            LTT_HashMap_Put(CloseEdge, Temp_VP, Temp_VDPP);
        }
        else
        {
            Temp_VDPP             = (VDPair*)malloc(sizeof(VDPair));
            Temp_VDPP->ConnectedP = VP;
            Temp_VDPP->Distance   = INT_MAX;
            LTT_HashMap_Put(CloseEdge, Temp_VP, Temp_VDPP);
        }
    }
    //将点到自己的距离设为0
    ((VDPair*)LTT_HashMap_Get(CloseEdge, VP))->Distance = 0;


    //开始Prim算法
    //每次找到CloseEdge中距离最小的点，将其距离设为0
    //然后更新CloseEdge
    //直到CloseEdge中所有的点的距离都为0


    for (int i = 1; i <= GP->VertexNum - 1; ++i)
    {
        //找到CloseEdge中距离Index最近的点
        int            Temp              = INT_MAX;
        AMLVertexNode* FindMin           = NULL;    //记录新找到的距离最小的顶点的指针
        Iterator       CloseEdgeIterator = LTT_HashMap_GetIterator(CloseEdge);
        while (CloseEdgeIterator.MoveNext(&CloseEdgeIterator))
        {
            Temp_VDPP = (VDPair*)LTT_HashMap_GetCurrent_Value(&CloseEdgeIterator);
            if (Temp_VDPP->Distance != 0 && Temp_VDPP->Distance < Temp)
            {
                //更新Temp和FindMin
                Temp    = Temp_VDPP->Distance;
                FindMin = (AMLVertexNode*)LTT_HashMap_GetCurrent_Key(&CloseEdgeIterator);
            }
        }
        //将FindMin的距离设为0
        Temp_VDPP              = (VDPair*)LTT_HashMap_Get(CloseEdge, FindMin);
        Temp_VDPP->Distance    = 0;


        //记录最小生成树的边
        Temp_EP                = LTT_Undigraph_Edge_Exist_by_ID(GP, FindMin->VertexID, Temp_VDPP->ConnectedP->VertexID);
        MiTreeEdgeArray[i - 1] = LTT_Undigraph_Make_Edge_byVP_Unsafe(Temp_EP->IVertex, Temp_EP->JVertex, Temp_EP->Data, Temp_EP->DataSize);


        //更新CloseEdge
        //遍历CloseEdge，找到所有与FindMin相连的点，更新其距离
        CloseEdgeIterator.Reset(&CloseEdgeIterator);
        while (CloseEdgeIterator.MoveNext(&CloseEdgeIterator))
        {
            Temp_VDPP = (VDPair*)LTT_HashMap_GetCurrent_Value(&CloseEdgeIterator);
            if (Temp_VDPP->Distance != 0 && (Temp_EP = LTT_Undigraph_Edge_Exist_by_ID(GP, FindMin->VertexID, ((AMLVertexNode*)LTT_HashMap_GetCurrent_Key(&CloseEdgeIterator))->VertexID)) != NULL && *(int*)Temp_EP->Data < Temp_VDPP->Distance)
            {
                Temp_VDPP->Distance   = *(int*)Temp_EP->Data;
                Temp_VDPP->ConnectedP = FindMin;
            }
        }
    }
    AMLUndigraph* MiTreep = LTT_Undigraph_New();
    LTT_Undigraph_Insert_VertexS(MiTreep, MiTreeVertexArray, GP->VertexNum);
    LTT_Undigraph_Insert_EdgeS(MiTreep, MiTreeEdgeArray, GP->VertexNum - 1);
    LTT_HashMap_Destroy(&CloseEdge);
    free(MiTreeEdgeArray);
    free(MiTreeVertexArray);
    return MiTreep;
}

AMLUndigraph* LTT_Undigraph_MiniSpanTree_Kruskal(const AMLUndigraph* const GP, const AMLVertexNode* const VP, const CompareFunction Comparator)
{
    if (LTT_Undigraph_Vertex_Exist_byVP(GP, VP) == false)
    {
        printf("顶点不存在\n");
        return NULL;
    }


    //判断是否联通
    if (LTT_Undigraph_Get_ConnectedComponentNumber(GP) > 1)
    {
        printf("图不连通\n");
        return NULL;
    }

    //用来存储最小生成树的边和顶点
    EdgeNode**      MiTreeEdgeArray   = (EdgeNode**)malloc((GP->VertexNum - 1) * sizeof(EdgeNode*));
    AMLVertexNode** MiTreeVertexArray = (AMLVertexNode**)malloc((GP->VertexNum) * sizeof(AMLVertexNode*));
    //用一个Edge数组来存储所有的边
    EdgeNode** AllEdgeArray           = (EdgeNode**)malloc(GP->EdgeNum * sizeof(EdgeNode*));
    if (!(MiTreeEdgeArray && MiTreeVertexArray && AllEdgeArray))
    {
        printf("内存分配失败\n");
        return NULL;
    }

    //用来临时存储顶点和边
    AMLVertexNode* Temp_VP  = NULL;
    EdgeNode*      Temp_EP  = NULL;

    //把图的顶点复制到MiTreeVertexArray
    Iterator VertexIterator = LTT_Undigraph_GetIterator_Vertex(GP);
    int      Temp_i         = 0;
    while (VertexIterator.MoveNext(&VertexIterator))
    {
        Temp_VP                     = LTT_Undigraph_GetCurrent_Vertex(&VertexIterator);
        MiTreeVertexArray[Temp_i++] = LTT_Undigraph_Make_Vertex(Temp_VP->VertexID, Temp_VP->Data, Temp_VP->DataSize);
    }


    //用一个Edge数组来存储所有的边
    //方便使用排序函数进行排序
    Iterator EdgeIterator = LTT_Undigraph_GetIterator_Edge(GP);
    int      Temp_j       = 0;
    while (EdgeIterator.MoveNext(&EdgeIterator))
    {
        Temp_EP                = LTT_Undigraph_GetCurrent_Edge(&EdgeIterator);
        AllEdgeArray[Temp_j++] = Temp_EP;
    }

    //调用排序函数对AllEdgeArray进行排序
    qsort(AllEdgeArray, GP->EdgeNum, sizeof(EdgeNode*), Comparator);


    //在Kruskal算法里面，首先将每个Edge按照权重大小进行排序
    //然后每次从边集中取出权重最小且两个顶点都不在同一个集合的边加入生成树中
    //使用并查集来防止构成环,并查集里面装的是顶点的ID

    UnionFindSet* UFSet = LTT_UnionFindSet_New(VID_MAX_LENGTH * sizeof(char), NULL);
    int           Count = 0;                               //用来记录生成树中的边的数量
    for (int i = 0; i < GP->EdgeNum; ++i)
    {
        char* VIP = AllEdgeArray[i]->IVertex->VertexID;    //指向IvertexID的指针
        char* VJP = AllEdgeArray[i]->JVertex->VertexID;    //指向JvertexID的指针
        //将两个顶点加入并查集中
        LTT_UnionFindSet_Put(UFSet, VIP);
        LTT_UnionFindSet_Put(UFSet, VJP);
        if (LTT_UnionFindSet_IsUnite(UFSet, VIP, VJP)) continue;    //如果两个顶点在同一个集合中，就跳过
        //将两个顶点合并到同一个集合中
        LTT_UnionFindSet_Unite(UFSet, VIP, VJP);
        //将此边加入生成树中
        MiTreeEdgeArray[Count++] = LTT_Undigraph_Make_Edge_byVP_Unsafe(AllEdgeArray[i]->IVertex, AllEdgeArray[i]->JVertex, AllEdgeArray[i]->Data, AllEdgeArray[i]->DataSize);
    }
    AMLUndigraph* MiTreeP = LTT_Undigraph_New();
    LTT_Undigraph_Insert_VertexS(MiTreeP, MiTreeVertexArray, GP->VertexNum);
    LTT_Undigraph_Insert_EdgeS(MiTreeP, MiTreeEdgeArray, GP->VertexNum - 1);
    free(MiTreeEdgeArray);
    free(MiTreeVertexArray);
    free(AllEdgeArray);
    return MiTreeP;
}

int LTT_Undigraph_Get_VertexIndex_by_VP(const AMLUndigraph* const GP, const AMLVertexNode* const VP)
{
    LTT_Undigraph_Make_VertexArray(GP);
    for (int i = 1; i <= GP->VertexNum; ++i)
    {
        if (__VertexArray[i] == VP) return i;
    }
    return 0;
}

int LTT_Undigraph_Get_VertexIndex_by_ID(const AMLUndigraph* const GP, const char* const VertexID)
{
    LTT_Undigraph_Make_VertexArray(GP);
    for (int i = 1; i <= GP->VertexNum; ++i)
    {
        if (strcmp(__VertexArray[i]->VertexID, VertexID) == 0) return i;
    }
    return 0;
}

Status LTT_Undigraph_ShortestPath_Dijkstra(const AMLUndigraph* const GP, const char* const VertexID, int** DisMatrix, int** Path)
{
    (*DisMatrix) = (int*)calloc(GP->VertexNum + 1, sizeof(int));
    (*Path)      = (int*)calloc(GP->VertexNum + 1, sizeof(int));
    if (!(*DisMatrix) || !(*Path)) return OVERFLOW;
    //初始化DisMatrix和Path
    //初始化所有顶点的访问性
    Iterator VertexIterator = LTT_Undigraph_GetIterator_Vertex(GP);
    for (int i = 1; VertexIterator.MoveNext(&VertexIterator); ++i)
    {
        (*DisMatrix)[i]                                        = INT_MAX;
        (*Path)[i]                                             = 0;
        LTT_Undigraph_GetCurrent_Vertex(&VertexIterator)->Mark = Unvisited;
    }

    //找到顶点VertexID对应的Vindex
    int Vindex                  = LTT_Undigraph_Get_VertexIndex_by_ID(GP, VertexID);
    (*DisMatrix)[Vindex]        = 0;
    Path[Vindex]                = 0;
    __VertexArray[Vindex]->Mark = Visited;
    int MinIndex                = Vindex;
    for (int i = 1; i <= GP->VertexNum - 1; ++i)
    {
        EdgeNode* Iterator = __VertexArray[MinIndex]->FirstEdge;
        while (Iterator)
        {
            if (MinIndex == LTT_Undigraph_Get_VertexIndex_by_VP(GP, Iterator->IVertex))
            {
                if (__VertexArray[LTT_Undigraph_Get_VertexIndex_by_VP(GP, Iterator->JVertex)]->Mark == Unvisited && (*DisMatrix)[LTT_Undigraph_Get_VertexIndex_by_VP(GP, Iterator->JVertex)] > (*DisMatrix)[MinIndex] + *(int*)Iterator->Data)
                {
                    (*DisMatrix)[LTT_Undigraph_Get_VertexIndex_by_VP(GP, Iterator->JVertex)] = (*DisMatrix)[MinIndex] + *(int*)Iterator->Data;
                    (*Path)[LTT_Undigraph_Get_VertexIndex_by_VP(GP, Iterator->JVertex)]      = MinIndex;
                }
                Iterator = Iterator->IEdge;
            }
            else
            {
                if (__VertexArray[LTT_Undigraph_Get_VertexIndex_by_VP(GP, Iterator->IVertex)]->Mark == Unvisited && (*DisMatrix)[LTT_Undigraph_Get_VertexIndex_by_VP(GP, Iterator->IVertex)] > (*DisMatrix)[MinIndex] + *(int*)Iterator->Data)
                {
                    (*DisMatrix)[LTT_Undigraph_Get_VertexIndex_by_VP(GP, Iterator->IVertex)] = (*DisMatrix)[MinIndex] + *(int*)Iterator->Data;
                    (*Path)[LTT_Undigraph_Get_VertexIndex_by_VP(GP, Iterator->IVertex)]      = MinIndex;
                }
                Iterator = Iterator->JEdge;
            }
        }
        int MinDistance = INT_MAX;
        for (int i = 1; i <= GP->VertexNum; ++i)
        {
            if (__VertexArray[i]->Mark == Unvisited && (*DisMatrix)[i] <= MinDistance)
            {
                MinDistance = (*DisMatrix)[i];
                MinIndex    = i;
            }
        }
        __VertexArray[MinIndex]->Mark = Visited;
    }
    return OK;
}

Status LTT_Undigraph_ShortestPath_Floyd_Warshall(const AMLUndigraph* const GP, int*** DisMatrix, int*** Path)
{
    if (!((*Path) = (int**)calloc(GP->VertexNum + 1, sizeof(int*)))) return OVERFLOW;
    for (int i = 0; i <= GP->VertexNum; ++i) (*Path)[i] = (int*)calloc(GP->VertexNum + 1, sizeof(int));
    //初始化带权邻接矩阵
    (*DisMatrix) = LTT_Undigraph_Make_Weighted_AdjMatrix(GP);
    for (int i = 1; i <= GP->VertexNum; ++i)
    {
        for (int j = 1; j <= GP->VertexNum; ++j)
        {
            if ((*DisMatrix)[i][j] != INT_MAX) (*Path)[i][j] = i;
            else if (i != j) (*Path)[i][j] = -1;
        }
    }
    for (int i = 1; i <= GP->VertexNum; ++i) (*DisMatrix)[i][i] = 0;
    for (int k = 1; k <= GP->VertexNum; ++k)
    {
        for (int i = 1; i <= GP->VertexNum; ++i)    //对i与j两个顶点之间，判断是当前距离短还是经过一点k更短
        {
            for (int j = 1; j <= GP->VertexNum; ++j)
            {
                if ((*DisMatrix)[i][k] == INT_MAX || (*DisMatrix)[k][j] == INT_MAX) continue;
                if ((*DisMatrix)[i][j] > (*DisMatrix)[i][k] + (*DisMatrix)[k][j])
                {
                    (*DisMatrix)[i][j] = (*DisMatrix)[i][k] + (*DisMatrix)[k][j];
                    (*Path)[i][j]      = (*Path)[k][j];
                }
                if ((*DisMatrix)[i][i] < 0)
                {
                    printf("存在负闭环");
                    return ERROR;
                }
            }
        }
    }
    for (int i = 1; i <= GP->VertexNum; ++i) { printf("%s:%d\n", __VertexArray[i]->VertexID, i); }
    return OK;
}
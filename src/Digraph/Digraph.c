#include "Digraph.h"

Status CreateDigraph(OLDigraphPointer GP, OLVertexNode* OLVertexArray, int OLVertexNum, ArcNode* ArcArray, int ArcNum)
{
    if (OLVertexNum > MAX_VERTEX_NUM) return ERROR;
    GP->ArcNum      = 0;
    GP->OLVertexNum = OLVertexNum;
    for (int i = 0; i < OLVertexNum; ++i) { GP->OLVertexList[i + 1] = OLVertexArray[i]; }
    for (int i = 0; i < ArcNum; ++i) { InsertArc(GP, GP->OLVertexList + ArcArray[i].TailVertex, GP->OLVertexList + ArcArray[i].HeadVertex, ArcArray[i].Data, ArcArray[i].DataSize); }
    return OK;
}

Status CreateAdjMatrix_OL(OLDigraph G, int*** AdjMatrix)
{
    if (!(*AdjMatrix = (int**)calloc(G.OLVertexNum + 1, sizeof(int*)))) return OVERFLOW;
    for (int i = 0; i <= G.OLVertexNum; ++i) (*AdjMatrix)[i] = (int*)calloc(G.OLVertexNum + 1, sizeof(int));
    for (int i = 1; i <= G.OLVertexNum; ++i)
    {
        for (int j = 1; j <= G.OLVertexNum; ++j)
        {
            ArcNodePointer ArcP = LocateArcPosition(G, i, j);
            if (ArcP != NULL)
            {
                (*AdjMatrix)[i][j] = *(int*)ArcP->Data;
                ++(*AdjMatrix)[i][0];
                ++(*AdjMatrix)[0][j];
            }
            else (*AdjMatrix)[i][j] = INT_MAX;
        }
    }
    return OK;
}

int OLVertexNodeExist(OLDigraphPointer GP, OLVertexNodePointer VP)
{
    if (VP == NULL) return 0;
    for (int i = 1; i <= GP->OLVertexNum; ++i)
    {
        if (GP->OLVertexList + i == VP) return i;
    }
    return 0;
}

Status MakeOLVertexNode(OLVertexNodePointer VP, void* Data, size_t DataSize)
{
    VP->DataSize = DataSize;
    if (!(VP->Data = (void*)malloc(DataSize))) return OVERFLOW;
    memcpy(VP->Data, Data, DataSize);
    VP->FirstIn  = NULL;
    VP->FirstOut = NULL;
    return OK;
}

Status MakeArcNode(ArcNodePointer AP, int TailVertex, int HeadVertex, void* Data, size_t DataSize)
{
    AP->DataSize = DataSize;
    if (!(AP->Data = (void*)malloc(DataSize))) return OVERFLOW;
    memcpy(AP->Data, Data, DataSize);
    AP->HeadVertex = HeadVertex;
    AP->TailVertex = TailVertex;
    AP->HeadArc    = NULL;
    AP->TailArc    = NULL;
}

Status DestroyDigraph(OLDigraphPointer GP)
{
    ArcTraverse(*GP, FreeArc);
    for (int i = 1; i <= GP->OLVertexNum; ++i) { free(GP->OLVertexList[i].Data); }
    GP = NULL;
    return OK;
}

void*  GetOLVertex(OLDigraphPointer GP, OLVertexNodePointer VP) { return (OLVertexNodeExist(GP, VP)) ? VP->Data : NULL; }

Status PutOLVertex(OLDigraphPointer GP, OLVertexNodePointer VP, void* Data, size_t DataSize)
{
    if (!OLVertexNodeExist(GP, VP)) return ERROR;
    VP->DataSize = DataSize;
    memcpy(VP->Data, Data, DataSize);
    return OK;
}

OLVertexNodePointer FirstAdjOLVertex(OLDigraphPointer GP, OLVertexNodePointer VP)
{
    int Vid = OLVertexNodeExist(GP, VP);
    if (!Vid) return NULL;
    else if (GP->OLVertexList[Vid].FirstOut == NULL) return NULL;
    else return GP->OLVertexList + VP->FirstOut->HeadVertex;
}

OLVertexNodePointer FristUnvisitedAdjOLVertex(OLDigraphPointer GP, OLVertexNodePointer VP)
{
    int Vid = OLVertexNodeExist(GP, VP);
    if (!Vid) return NULL;
    else if (GP->OLVertexList[Vid].FirstOut == NULL) return NULL;
    ArcNodePointer Iterator = GP->OLVertexList[Vid].FirstOut;
    while (Iterator)
    {
        if (GP->OLVertexList[Iterator->HeadVertex].Mark == Unvisited) return GP->OLVertexList + Iterator->HeadVertex;
        else Iterator = Iterator->TailArc;
    }
    return NULL;
}

Status InsertOLVertex(OLDigraphPointer GP, OLVertexNode V)
{
    if (GP->OLVertexNum == MAX_VERTEX_NUM) return OVERFLOW;
    ++GP->OLVertexNum;
    GP->OLVertexList[GP->OLVertexNum] = V;
    return OK;
}

Status DeleteOLVertex(OLDigraphPointer GP, OLVertexNodePointer VP)
{
    int Vid = OLVertexNodeExist(GP, VP);
    if (!Vid) return ERROR;
    --GP->OLVertexNum;
    if (VP->FirstIn != NULL)
    {
        ArcNodePointer Iterator = GP->OLVertexList[Vid].FirstIn;
        while (Iterator)
        {
            ArcNodePointer  Temp        = Iterator;
            ArcNodePointer* PriorEdgePP = GetPriorArcPP(GP, Vid, Iterator);
            *PriorEdgePP                = Iterator->HeadArc;
            Iterator                    = Iterator->HeadArc;
            free(Temp->Data);
            free(Temp);
            --GP->ArcNum;
        }
    }
    if (VP->FirstOut != NULL)
    {
        ArcNodePointer Iterator = GP->OLVertexList[Vid].FirstOut;
        while (Iterator)
        {
            ArcNodePointer  Temp        = Iterator;
            ArcNodePointer* PriorEdgePP = GetPriorArcPP(GP, Vid, Iterator);
            *PriorEdgePP                = Iterator->TailArc;
            Iterator                    = Iterator->TailArc;
            free(Temp->Data);
            free(Temp);
            --GP->ArcNum;
        }
    }
    return OK;
}

ArcNodePointer* GetPriorArcPP(OLDigraphPointer GP, int Vid, ArcNodePointer AP)
{
    if (Vid < 1 || Vid > GP->OLVertexNum) return NULL;
    if (AP->HeadVertex == Vid)
    {
        if (GP->OLVertexList[Vid].FirstIn == AP) return &(GP->OLVertexList[Vid].FirstIn);
        ArcNodePointer Iterator = GP->OLVertexList[Vid].FirstIn;
        while (Iterator)
        {
            if (AP == Iterator->HeadArc) return &(Iterator->HeadArc);
        }
    }
    else
    {
        if (GP->OLVertexList[Vid].FirstOut == AP) return &(GP->OLVertexList[Vid].FirstOut);
        ArcNodePointer Iterator = GP->OLVertexList[Vid].FirstOut;
        while (Iterator)
        {
            if (AP == Iterator->TailArc) return &(Iterator->TailArc);
        }
    }
    return NULL;
}

ArcNodePointer* GetTailInArcPP(OLDigraphPointer GP, int Vid)
{
    if (Vid < 1 || Vid > GP->OLVertexNum) return NULL;
    if (GP->OLVertexList[Vid].FirstIn == NULL) return &(GP->OLVertexList[Vid].FirstIn);
    else
    {
        ArcNodePointer Iterator = GP->OLVertexList[Vid].FirstIn;
        while (Iterator->HeadArc != NULL) { Iterator = Iterator->HeadArc; }
        return &(Iterator->HeadArc);
    }
    return NULL;
}

ArcNodePointer* GetTailOutArcPP(OLDigraphPointer GP, int Vid)
{
    if (Vid < 1 || Vid > GP->OLVertexNum) return NULL;
    if (GP->OLVertexList[Vid].FirstOut == NULL) return &(GP->OLVertexList[Vid].FirstOut);
    else
    {
        ArcNodePointer Iterator = GP->OLVertexList[Vid].FirstOut;
        while (Iterator->TailArc != NULL) { Iterator = Iterator->TailArc; }
        return &(Iterator->TailArc);
    }
    return NULL;
}

ArcNodePointer LocateArcPosition(OLDigraph G, int TailVertex, int HeadVertex)
{
    if (!(TailVertex && HeadVertex)) return NULL;
    if (TailVertex == HeadVertex) return NULL;
    if (TailVertex > G.OLVertexNum || HeadVertex > G.OLVertexNum) return NULL;
    if (G.OLVertexList[TailVertex].FirstOut == NULL) return NULL;
    ArcNodePointer Iterator = G.OLVertexList[TailVertex].FirstOut;
    while (Iterator)
    {
        if (Iterator->HeadVertex == HeadVertex) return Iterator;
        else Iterator = Iterator->TailArc;
    }
    return NULL;
}

Status InsertArc(OLDigraphPointer GP, OLVertexNodePointer TailP, OLVertexNodePointer HeadP, void* Data, size_t DataSize)
{
    int Tailid = OLVertexNodeExist(GP, TailP);
    int Headid = OLVertexNodeExist(GP, HeadP);
    if (!(Tailid && Headid)) return ERROR;
    if (LocateArcPosition(*GP, Tailid, Headid)) return ERROR;
    ++GP->ArcNum;
    ArcNodePointer* TailLastOutArcPP = GetTailOutArcPP(GP, Tailid);
    ArcNodePointer* HaedLastInArcPP  = GetTailInArcPP(GP, Headid);
    if (!((*TailLastOutArcPP) = (ArcNodePointer)malloc(sizeof(ArcNode)))) return OVERFLOW;
    (*HaedLastInArcPP) = (*TailLastOutArcPP);
    if (!((*TailLastOutArcPP)->Data = (void*)malloc(DataSize))) return OVERFLOW;
    memcpy((*TailLastOutArcPP)->Data, Data, DataSize);
    (*TailLastOutArcPP)->DataSize   = DataSize;
    (*TailLastOutArcPP)->HeadVertex = Headid;
    (*TailLastOutArcPP)->TailVertex = Tailid;
    (*TailLastOutArcPP)->HeadArc    = NULL;
    (*TailLastOutArcPP)->TailArc    = NULL;
    return OK;
}

Status DeleteArc(OLDigraphPointer GP, OLVertexNodePointer TailP, OLVertexNodePointer HeadP)
{
    int Tailid = OLVertexNodeExist(GP, TailP);
    int Headid = OLVertexNodeExist(GP, HeadP);
    if (!(Tailid && Headid)) return ERROR;
    ArcNodePointer Tail2Head = LocateArcPosition(*GP, Tailid, Headid);
    if (Tail2Head == NULL) return ERROR;
    --GP->ArcNum;
    ArcNodePointer* TailPriorArcPP  = GetPriorArcPP(GP, Tailid, Tail2Head);
    ArcNodePointer* HeadPriorEdgePP = GetPriorArcPP(GP, Headid, Tail2Head);
    *TailPriorArcPP                 = Tail2Head->TailArc;
    *HeadPriorEdgePP                = Tail2Head->HeadArc;
    free(Tail2Head->Data);
    free(Tail2Head);
    return OK;
}

Status ArcTraverse(OLDigraph G, Status (*Visit)(ArcNodePointer))
{
    for (int i = 1; i <= G.OLVertexNum; ++i)
    {
        if (G.OLVertexList[i].FirstIn != NULL)
        {
            ArcNodePointer Iterator = G.OLVertexList[i].FirstIn;
            while (Iterator)
            {
                Iterator->Mark = Unvisited;
                Iterator       = Iterator->HeadArc;
            }
        }
        else if (G.OLVertexList[i].FirstOut != NULL)
        {
            ArcNodePointer Iterator = G.OLVertexList[i].FirstOut;
            while (Iterator)
            {
                Iterator->Mark = Unvisited;
                Iterator       = Iterator->TailArc;
            }
        }
    }
    for (int i = 1; i <= G.OLVertexNum; ++i)
    {
        if (G.OLVertexList[i].FirstIn != NULL)
        {
            ArcNodePointer Iterator = G.OLVertexList[i].FirstIn;
            while (Iterator)
            {
                if (Iterator->Mark == Unvisited)
                {
                    (*Visit)(Iterator);
                    Iterator->Mark = Visited;
                }
                Iterator = Iterator->HeadArc;
            }
        }
        else if (G.OLVertexList[i].FirstOut != NULL)
        {
            ArcNodePointer Iterator = G.OLVertexList[i].FirstOut;
            while (Iterator)
            {
                if (Iterator->Mark == Unvisited)
                {
                    (*Visit)(Iterator);
                    Iterator->Mark = Visited;
                }
                Iterator = Iterator->TailArc;
            }
        }
    }
    return OK;
}

Status FreeArc(ArcNodePointer AP)
{
    free(AP->Data);
    return OK;
}

Status DFSTraverse_OL_Recursive(OLDigraphPointer GP, int Vid, Status (*Visit)(OLVertexNodePointer))
{
    //初始化所有的点的可访问性
    for (int i = 1; i <= GP->OLVertexNum; ++i) { GP->OLVertexList[i].Mark = Unvisited; }
    //从第vid个点出发递归调用；
    DFS_OL(GP, Vid, Visit);
    return OK;
}

void DFS_OL(OLDigraphPointer GP, int Vid, Status (*Visit)(OLVertexNodePointer))
{
    GP->OLVertexList[Vid].Mark = Visited;
    (*Visit)(GP->OLVertexList + Vid);
    int NextVid = OLVertexNodeExist(GP, FristUnvisitedAdjOLVertex(GP, GP->OLVertexList + Vid));
    while (NextVid)
    {
        DFS_OL(GP, NextVid, Visit);
        NextVid = OLVertexNodeExist(GP, FristUnvisitedAdjOLVertex(GP, GP->OLVertexList + Vid));
    }
}

Status DFSTraverse_OL_Stack(OLDigraphPointer GP, int Vid, Status (*Visit)(OLVertexNodePointer))
{
    //初始化所有的点的可访问性
    for (int i = 1; i <= GP->OLVertexNum; ++i) { GP->OLVertexList[i].Mark = Unvisited; }
    SqStack Stack;
    InitStack(&Stack, sizeof(int));
    Push(&Stack, &Vid);
    while (!StackEmpty(Stack))
    {
        int Temp;
        Pop(&Stack, &Temp);
        (*Visit)(GP->OLVertexList + Temp);
        GP->OLVertexList[Temp].Mark = Visited;
        int NextVid                 = OLVertexNodeExist(GP, FristUnvisitedAdjOLVertex(GP, GP->OLVertexList + Temp));
        while (NextVid)
        {
            Push(&Stack, &NextVid);
            GP->OLVertexList[NextVid].Mark = Visited;
            NextVid                        = OLVertexNodeExist(GP, FristUnvisitedAdjOLVertex(GP, GP->OLVertexList + Temp));
        }
    }
}

Status BFSTraverse_OL(OLDigraphPointer GP, int Vid, Status (*Visit)(OLVertexNodePointer))
{
    //初始化所有的点的可访问性
    for (int i = 1; i <= GP->OLVertexNum; ++i) { GP->OLVertexList[i].Mark = Unvisited; }
    SqQueue Queue;
    InitQueue(&Queue, sizeof(int));
    EnQueue(&Queue, &Vid);
    while (!QueueEmpty(Queue))
    {
        int Temp;
        DeQueue(&Queue, &Temp);
        (*Visit)(GP->OLVertexList + Temp);
        GP->OLVertexList[Temp].Mark = Visited;
        int NextVid                 = OLVertexNodeExist(GP, FristUnvisitedAdjOLVertex(GP, GP->OLVertexList + Temp));
        while (NextVid)
        {
            EnQueue(&Queue, &NextVid);
            GP->OLVertexList[NextVid].Mark = Visited;
            NextVid                        = OLVertexNodeExist(GP, FristUnvisitedAdjOLVertex(GP, GP->OLVertexList + Temp));
        }
    }
}

int StronglyConnectedComponent_OL(OLDigraphPointer GP)
{
    int     DFN[MAX_VERTEX_NUM + 1];    // 存放节点的DFS序号
    int     LOW[MAX_VERTEX_NUM + 1];    // 存放节点的最早追溯祖先的DFS序号
    int     SCC[MAX_VERTEX_NUM + 1];    // 存放每个点所属的强连通分量编号
    int     SCCNum;                     // 强连通分量的数量
    int     Index;                      // 记录DFS的遍历顺序
    SqStack Stack;
    InitStack(&Stack, sizeof(int));
    bool InStack[MAX_VERTEX_NUM];       // 用于判断节点是否在栈中
    SCCNum = 0;
    Index  = 0;
    for (int i = 1; i <= GP->OLVertexNum; i++)
    {
        DFN[i]     = 0;
        InStack[i] = false;
    }
    for (int i = 1; i <= GP->OLVertexNum; i++)
    {
        if (DFN[i] == 0) Tarjan(GP, i, DFN, LOW, SCC, &SCCNum, &Index, InStack, &Stack);
    }
    return SCCNum;
}

void Tarjan(OLDigraphPointer GP, int Vid, int* DFN, int* LOW, int* SCC, int* SCCNum, int* Index, int* InStack, SqStackPointer StackPointer)
{
    DFN[Vid] = LOW[Vid] = ++(*Index);    //初始化起始顶点的DFN和Low值
    Push(StackPointer, &Vid);            //将当前节点入栈
    InStack[Vid]      = true;            //表示该节点在栈中
    ArcNodePointer AP = GP->OLVertexList[Vid].FirstOut;
    while (AP)
    {
        int Wid = AP->HeadVertex;
        if (DFN[Wid] == 0)
        {
            Tarjan(GP, Wid, DFN, LOW, SCC, SCCNum, Index, InStack, StackPointer);
            LOW[Vid] = (LOW[Vid] <= LOW[Wid]) ? LOW[Vid] : LOW[Wid];
        }
        else if (InStack[Wid]) LOW[Vid] = (LOW[Vid] <= DFN[Wid]) ? LOW[Vid] : DFN[Wid];
        AP = AP->TailArc;
    }
    if (DFN[Vid] == LOW[Vid])
    {
        (*SCCNum)++;
        int TopVid;
        do {
            Pop(StackPointer, &TopVid);
            InStack[TopVid] = false;
            SCC[TopVid]     = SCCNum;
        }
        while (TopVid != Vid);
    }
}

Status TopologicalSort(OLDigraphPointer GP)
{
    int* Indegree = (int*)malloc((GP->OLVertexNum + 1) * sizeof(int));
    for (int i = 1; i <= GP->OLVertexNum; ++i)
    {
        int Count = 0;
        if (GP->OLVertexList[i].FirstIn == NULL) Indegree[i] = Count;
        else
        {
            ArcNodePointer Iterator = GP->OLVertexList[i].FirstIn;
            ++Count;
            while (Iterator->HeadArc != NULL)
            {
                ++Count;
                Iterator = Iterator->HeadArc;
            }
            Indegree[i] = Count;
        }
    }
    SqStack Stack;
    InitStack(&Stack, sizeof(int));
    for (int i = 1; i <= GP->OLVertexNum; ++i)
        if (!Indegree[i]) Push(&Stack, &i);
    int Count = 0;
    while (!StackEmpty(Stack))
    {
        int Temp;
        Pop(&Stack, &Temp);
        printf("%d", Temp);    //拓扑排序的位置
        ++Count;
        for (ArcNodePointer P = GP->OLVertexList[Temp].FirstOut; P; P = P->TailArc)
        {
            int NextId = P->HeadVertex;
            if (!(--Indegree[NextId])) Push(&Stack, &NextId);
        }
    }
    if (Count < GP->OLVertexNum) return ERROR;
    return OK;
}

Status CriticalPath(OLDigraphPointer GP)
{
    int*    VE = (int*)calloc((GP->OLVertexNum + 1), sizeof(int));
    int*    VL = (int*)calloc((GP->OLVertexNum + 1), sizeof(int));
    int*    EE = (int*)calloc((GP->OLVertexNum + 1), sizeof(int));
    int*    EL = (int*)calloc((GP->OLVertexNum + 1), sizeof(int));
    SqStack TopologicalSortStack;
    InitStack(&TopologicalSortStack, sizeof(int));
    if (!CriticalPath_Kernel(GP, &TopologicalSortStack, VE)) return ERROR;
    for (int i = 1; i <= GP->OLVertexNum; ++i) VL[i] = VE[GP->OLVertexNum];
    while (!StackEmpty(TopologicalSortStack))
    {
        int Temp;
        Pop(&TopologicalSortStack, &Temp);
        for (ArcNodePointer P = GP->OLVertexList[Temp].FirstOut; P; P = P->TailArc)
        {
            int NextId = P->HeadVertex;
            int Dut    = *(int*)P->Data;
            VL[Temp]   = (VL[NextId] - Dut < VL[Temp]) ? VL[NextId] - Dut : VL[Temp];
        }
    }
    for (int i = 1; i <= GP->OLVertexNum; ++i)
    {
        for (ArcNodePointer P = GP->OLVertexList[i].FirstOut; P; P = P->TailArc)
        {
            int NextId = P->HeadVertex;
            int Dut    = *(int*)P->Data;
            EE[i]      = VE[i];
            EL[i]      = VL[NextId] - Dut;
            if (EE[i] == EL[i]) printf("%dto%d\n", i, NextId);    //关键路径
        }
    }
    return OK;
}

Status CriticalPath_Kernel(OLDigraphPointer GP, SqStackPointer TopologicalSortStackP, int* VE)
{
    int* Indegree = (int*)malloc((GP->OLVertexNum + 1) * sizeof(int));
    for (int i = 1; i <= GP->OLVertexNum; ++i)
    {
        int Count = 0;
        if (GP->OLVertexList[i].FirstIn == NULL) Indegree[i] = Count;
        else
        {
            ArcNodePointer Iterator = GP->OLVertexList[i].FirstIn;
            ++Count;
            while (Iterator->HeadArc != NULL)
            {
                ++Count;
                Iterator = Iterator->HeadArc;
            }
            Indegree[i] = Count;
        }
    }
    SqStack Stack;
    InitStack(&Stack, sizeof(int));
    for (int i = 1; i <= GP->OLVertexNum; ++i)
        if (!Indegree[i]) Push(&Stack, &i);
    int Count = 0;
    while (!StackEmpty(Stack))
    {
        int Temp;
        Pop(&Stack, &Temp);
        Push(TopologicalSortStackP, &Temp);
        ++Count;
        for (ArcNodePointer P = GP->OLVertexList[Temp].FirstOut; P; P = P->TailArc)
        {
            int NextId = P->HeadVertex;
            if (!(--Indegree[NextId])) Push(&Stack, &NextId);
            VE[NextId] = (VE[Temp] + *(int*)(P->Data) > VE[NextId]) ? VE[Temp] + *(int*)(P->Data) : VE[NextId];
        }
    }
    if (Count < GP->OLVertexNum) return ERROR;
    return OK;
}

bool HasCircle_OL(OLDigraph G)
{
    int* Indegree = (int*)malloc((G.OLVertexNum + 1) * sizeof(int));
    for (int i = 1; i <= G.OLVertexNum; ++i)
    {
        int Count = 0;
        if (G.OLVertexList[i].FirstIn == NULL) Indegree[i] = Count;
        else
        {
            ArcNodePointer Iterator = G.OLVertexList[i].FirstIn;
            ++Count;
            while (Iterator->HeadArc != NULL)
            {
                ++Count;
                Iterator = Iterator->HeadArc;
            }
            Indegree[i] = Count;
        }
    }
    SqStack Stack;
    InitStack(&Stack, sizeof(int));
    for (int i = 1; i <= G.OLVertexNum; ++i)
        if (!Indegree[i]) Push(&Stack, &i);
    int Count = 0;
    while (!StackEmpty(Stack))
    {
        int Temp;
        Pop(&Stack, &Temp);
        ++Count;
        for (ArcNodePointer P = G.OLVertexList[Temp].FirstOut; P; P = P->TailArc)
        {
            int NextId = P->HeadVertex;
            if (!(--Indegree[NextId])) Push(&Stack, &NextId);
        }
    }
    if (Count < G.OLVertexNum) return true;
    return false;
}

Status ShortestPath_OL_Dijkstra(OLDigraph G, int Vid)
{
    int* Distance = (int*)calloc(G.OLVertexNum + 1, sizeof(int));
    int* Path     = (int*)calloc(G.OLVertexNum + 1, sizeof(int));
    for (int i = 1; i <= G.OLVertexNum; ++i)
    {
        G.OLVertexList[i].Mark = Unvisited;
        Distance[i]            = INT_MAX;
        Path[i]                = -1;
    }
    Distance[Vid]            = 0;
    Path[Vid]                = 0;
    G.OLVertexList[Vid].Mark = Visited;
    int MinIndex             = Vid;
    for (int i = 1; i <= G.OLVertexNum - 1; ++i)
    {
        ArcNodePointer Iterator = G.OLVertexList[MinIndex].FirstOut;
        while (Iterator)
        {
            if (G.OLVertexList[Iterator->HeadVertex].Mark == Unvisited && Distance[Iterator->HeadVertex] > Distance[MinIndex] + *(int*)Iterator->Data)
            {
                Distance[Iterator->HeadVertex] = Distance[MinIndex] + *(int*)Iterator->Data;
                Path[Iterator->HeadVertex]     = MinIndex;
            }
            Iterator = Iterator->TailArc;
        }
        int MinDistance = INT_MAX;
        for (int i = 1; i <= G.OLVertexNum; ++i)
        {
            if (G.OLVertexList[i].Mark == Unvisited && Distance[i] <= MinDistance)
            {
                MinDistance = Distance[i];
                MinIndex    = i;
            }
        }
        G.OLVertexList[MinIndex].Mark = Visited;
    }
    for (int i = 1; i <= G.OLVertexNum; ++i) { printf("%d ", Distance[i]); }
}

Status ShortestPath_OL_Bellman_Ford(OLDigraph G, int Vid)
{
    int*            Distance    = (int*)calloc(G.OLVertexNum + 1, sizeof(int));
    int*            Path        = (int*)calloc(G.OLVertexNum + 1, sizeof(int));
    //将所有的Arc全部提取出来
    ArcNodePointer* AllArcArray = (ArcNodePointer*)malloc((G.ArcNum + 1) * sizeof(ArcNodePointer));
    int             Count       = 0;
    int             MaxData     = 0;    //为了防止数据溢出
    for (int i = 1; i <= G.OLVertexNum; ++i)
    {
        if (G.OLVertexList[i].FirstIn != NULL)
        {
            ArcNodePointer Iterator = G.OLVertexList[i].FirstIn;
            while (Iterator)
            {
                Iterator->Mark = Unvisited;
                MaxData        = (MaxData < *(int*)Iterator->Data) ? *(int*)Iterator->Data : MaxData;
                Iterator       = Iterator->HeadArc;
            }
        }
        else if (G.OLVertexList[i].FirstOut != NULL)
        {
            ArcNodePointer Iterator = G.OLVertexList[i].FirstOut;
            while (Iterator)
            {
                Iterator->Mark = Unvisited;
                MaxData        = (MaxData < *(int*)Iterator->Data) ? *(int*)Iterator->Data : MaxData;
                Iterator       = Iterator->TailArc;
            }
        }
    }
    for (int i = 1; i <= G.OLVertexNum; ++i)
    {
        Distance[i] = INT_MAX - MaxData;    //为了防止数据溢出
        Path[i]     = -1;
    }
    Distance[Vid] = 0;
    Path[Vid]     = 0;
    for (int i = 1; i <= G.OLVertexNum; ++i)
    {
        if (G.OLVertexList[i].FirstIn != NULL)
        {
            ArcNodePointer Iterator = G.OLVertexList[i].FirstIn;
            while (Iterator)
            {
                if (Iterator->Mark == Unvisited)
                {
                    ++Count;
                    AllArcArray[Count] = Iterator;
                    Iterator->Mark     = Visited;
                }
                Iterator = Iterator->HeadArc;
            }
        }
        else if (G.OLVertexList[i].FirstOut != NULL)
        {
            ArcNodePointer Iterator = G.OLVertexList[i].FirstOut;
            while (Iterator)
            {
                if (Iterator->Mark == Unvisited)
                {
                    ++Count;
                    AllArcArray[Count] = Iterator;
                    Iterator->Mark     = Visited;
                }
                Iterator = Iterator->TailArc;
            }
        }
    }
    //提取完毕
    for (int i = 1; i <= G.OLVertexNum - 1; ++i)
    {
        int ChangeCount = 0;
        //循环所有的边，进行收缩
        for (int j = 1; j <= G.ArcNum; ++j)
        {
            ArcNodePointer ArcP = AllArcArray[j];
            if (Distance[ArcP->HeadVertex] > Distance[ArcP->TailVertex] + *(int*)ArcP->Data)
            {
                ++ChangeCount;
                Distance[ArcP->HeadVertex] = Distance[ArcP->TailVertex] + *(int*)ArcP->Data;
                Path[ArcP->HeadVertex]     = ArcP->TailVertex;
            }
        }
        if (ChangeCount == 0)
        {
            for (int i = 1; i <= G.OLVertexNum; ++i) { printf("%d ", Distance[i]); }
            return OK;
        }
    }
    //存在负闭环
    printf("存在负闭环");
    return ERROR;
}

Status ShortestPath_OL_Bellman_Ford_SPFA(OLDigraph G, int Vid)
{
    int* Distance       = (int*)calloc(G.OLVertexNum + 1, sizeof(int));
    int* Path           = (int*)calloc(G.OLVertexNum + 1, sizeof(int));
    int* InQueueCounter = (int*)calloc(G.OLVertexNum + 1, sizeof(int));
    for (int i = 1; i <= G.OLVertexNum; ++i)
    {
        Distance[i] = INT_MAX;
        Path[i]     = -1;
    }
    for (int i = 1; i <= G.OLVertexNum; ++i) { G.OLVertexList[i].Mark = Unvisited; }
    Distance[Vid] = 0;
    Path[Vid]     = 0;
    SqQueue Queue;
    InitQueue(&Queue, sizeof(int));
    EnQueue(&Queue, &Vid);
    ++InQueueCounter[Vid];
    G.OLVertexList[Vid].Mark = Visited;
    while (!QueueEmpty(Queue))
    {
        int Temp;
        DeQueue(&Queue, &Temp);
        G.OLVertexList[Temp].Mark = Unvisited;
        ArcNodePointer Iterator   = G.OLVertexList[Temp].FirstOut;
        if (Iterator == NULL) continue;
        while (Iterator)
        {
            int NextId = Iterator->HeadVertex;
            if (Distance[NextId] > Distance[Temp] + *(int*)Iterator->Data)
            {
                Distance[NextId] = Distance[Temp] + *(int*)Iterator->Data;
                Path[NextId]     = Temp;
                if (G.OLVertexList[NextId].Mark == Unvisited)
                {
                    EnQueue(&Queue, &NextId);
                    ++InQueueCounter[NextId];
                    if (InQueueCounter[NextId] > G.OLVertexNum)    //存在负闭环
                    {
                        printf("存在负闭环");
                        return ERROR;
                    }
                    G.OLVertexList[NextId].Mark = Visited;
                }
            }
            Iterator = Iterator->TailArc;
        }
    }
    for (int i = 1; i <= G.OLVertexNum; ++i) { printf("%d ", Distance[i]); }
    return OK;
}

Status ShortestPath_OL_Floyd_Warshall(OLDigraph G)
{
    int** DisMatrix;
    int** Path;
    if (!(Path = (int**)calloc(G.OLVertexNum + 1, sizeof(int*)))) return OVERFLOW;
    for (int i = 0; i <= G.OLVertexNum; ++i) Path[i] = (int*)calloc(G.OLVertexNum + 1, sizeof(int));
    CreateAdjMatrix_OL(G, &DisMatrix);
    for (int i = 1; i <= G.OLVertexNum; ++i)
    {
        for (int j = 1; j <= G.OLVertexNum; ++j)
        {
            if (DisMatrix[i][j] != INT_MAX) Path[i][j] = i;
            else if (i != j) Path[i][j] = -1;
        }
    }
    for (int i = 1; i <= G.OLVertexNum; ++i) DisMatrix[i][i] = 0;
    for (int k = 1; k <= G.OLVertexNum; ++k)
    {
        for (int i = 1; i <= G.OLVertexNum; ++i)    //对i与j两个顶点之间，判断是当前距离短还是经过一点k更短
        {
            for (int j = 1; j <= G.OLVertexNum; ++j)
            {
                if (DisMatrix[i][k] == INT_MAX || DisMatrix[k][j] == INT_MAX) continue;
                if (DisMatrix[i][j] > DisMatrix[i][k] + DisMatrix[k][j])
                {
                    DisMatrix[i][j] = DisMatrix[i][k] + DisMatrix[k][j];
                    Path[i][j]      = Path[k][j];
                }
                if (DisMatrix[i][i] < 0)
                {
                    printf("存在负闭环");
                    return ERROR;
                }
            }
        }
    }
    for (int i = 1; i <= G.OLVertexNum; ++i)
    {
        for (int j = 1; j <= G.OLVertexNum; ++j)    //对i与j两个顶点之间，判断是当前距离短还是经过一点k更短
        {
            printf("%d ", Path[i][j]);
        }
        printf("\n");
    }
}
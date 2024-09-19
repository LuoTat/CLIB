#pragma once
#include "..\Graph\Predefined.h"
#include "..\LinearList\SqQueue.h"
#include "..\LinearList\SqStack.h"
#include "..\Predefined\Predefined.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ArcNode
{
    VisitIf         Mark;
    int             HeadVertex;
    int             TailVertex;
    struct ArcNode* HeadArc;
    struct ArcNode* TailArc;
    void*           Data;
    size_t          DataSize;
} ArcNode, *ArcNodePointer;

typedef struct
{
    VisitIf        Mark;
    ArcNodePointer FirstIn;
    ArcNodePointer FirstOut;
    void*          Data;
    size_t         DataSize;
} OLVertexNode, *OLVertexNodePointer;

typedef struct
{
    OLVertexNode OLVertexList[MAX_VERTEX_NUM + 1];
    int          OLVertexNum;
    int          ArcNum;
} OLDigraph, *OLDigraphPointer;

Status              CreateDigraph(OLDigraphPointer GP, OLVertexNode* OLVertexArray, int OLVertexNum, ArcNode* ArcArray, int ArcNum);
Status              CreateAdjMatrix_OL(OLDigraph G, int*** AdjMatrix);
int                 OLVertexNodeExist(OLDigraphPointer GP, OLVertexNodePointer VP);
Status              MakeOLVertexNode(OLVertexNodePointer VP, void* Data, size_t DataSize);
Status              MakeArcNode(ArcNodePointer AP, int HeadVertex, int TailVertex, void* Data, size_t DataSize);
Status              DestroyDigraph(OLDigraphPointer GP);
void*               GetOLVertex(OLDigraphPointer GP, OLVertexNodePointer VP);
Status              PutOLVertex(OLDigraphPointer GP, OLVertexNodePointer VP, void* Data, size_t DataSize);
OLVertexNodePointer FirstAdjOLVertex(OLDigraphPointer GP, OLVertexNodePointer VP);
OLVertexNodePointer FristUnvisitedAdjOLVertex(OLDigraphPointer GP, OLVertexNodePointer VP);
Status              InsertOLVertex(OLDigraphPointer GP, OLVertexNode V);
Status              DeleteOLVertex(OLDigraphPointer GP, OLVertexNodePointer VP);
ArcNodePointer*     GetPriorArcPP(OLDigraphPointer GP, int Vid, ArcNodePointer AP);
ArcNodePointer*     GetTailInArcPP(OLDigraphPointer GP, int Vid);
ArcNodePointer*     GetTailOutArcPP(OLDigraphPointer GP, int Vid);
ArcNodePointer      LocateArcPosition(OLDigraph G, int Vid, int Wid);
Status              InsertArc(OLDigraphPointer GP, OLVertexNodePointer VP, OLVertexNodePointer WP, void* Data, size_t DataSize);
Status              DeleteArc(OLDigraphPointer GP, OLVertexNodePointer VP, OLVertexNodePointer WP);
Status              ArcTraverse(OLDigraph G, Status (*Visit)(ArcNodePointer));
Status              FreeArc(ArcNodePointer AP);
Status              DFSTraverse_OL_Recursive(OLDigraphPointer GP, int Vid, Status (*Visit)(OLVertexNodePointer));
void                DFS_OL(OLDigraphPointer GP, int Vid, Status (*Visit)(OLVertexNodePointer));
Status              DFSTraverse_OL_Stack(OLDigraphPointer GP, int Vid, Status (*Visit)(OLVertexNodePointer));
Status              BFSTraverse_OL(OLDigraphPointer GP, int Vid, Status (*Visit)(OLVertexNodePointer));
int                 StronglyConnectedComponent_OL(OLDigraphPointer GP);
void                Tarjan(OLDigraphPointer GP, int Vid, int* DFN, int* Low, int* SCC, int* SCCNum, int* Index, int* InStack, SqStackPointer StackPointer);
Status              TopologicalSort(OLDigraphPointer GP);
Status              CriticalPath(OLDigraphPointer GP);
Status              CriticalPath_Kernel(OLDigraphPointer GP, SqStackPointer TopologicalSortStackP, int* VE);
bool                HasCircle_OL(OLDigraph G);
Status              ShortestPath_OL_Dijkstra(OLDigraph G, int Vid);
Status              ShortestPath_OL_Bellman_Ford(OLDigraph G, int Vid);
Status              ShortestPath_OL_Bellman_Ford_SPFA(OLDigraph G, int Vid);
Status              ShortestPath_OL_Floyd_Warshall(OLDigraph G);
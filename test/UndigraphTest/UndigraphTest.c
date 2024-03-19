#include <stdio.h>
#include <stdlib.h>

#include "LTT_Undigraph.h"

#define MAX_VERTEX_NUM 100
#define MAX_EDGE_NUM   MAX_VERTEX_NUM*(MAX_VERTEX_NUM - 1) / 2

int cmp(const void* const A, const void* const B) { return *(int*)LTT_Undigraph_Get_Data_From_Edge_byEP_Uncheck(*(EdgeNode**)A) - *(int*)LTT_Undigraph_Get_Data_From_Edge_byEP_Uncheck(*(EdgeNode**)B); }

Status VISIT(void* const V)
{
    printf("点：%s\n", (char*)LTT_Undigraph_Get_VertexID_byVP_Uncheck((AMLVertexNode*)V));
    return OK;
}

void PRINTG(AMLUndigraph* GP)
{
    Iterator VI = LTT_Undigraph_GetIterator_Vertex(GP);
    Iterator EI = LTT_Undigraph_GetIterator_Edge(GP);
    while (VI.MoveNext(&VI))
    {
        AMLVertexNode* V = LTT_Undigraph_GetCurrent_Vertex(&VI);
        printf("点：%s\n", (char*)LTT_Undigraph_Get_Data_From_Vertex_byVP(GP, V));
    }
    while (EI.MoveNext(&EI))
    {
        EdgeNode* E = LTT_Undigraph_GetCurrent_Edge(&EI);
        printf("边：%s\n", (char*)LTT_Undigraph_Get_Data_From_Edge_byEP(GP, E));
    }
}

int main()
{
    AMLUndigraph*  G = NULL;
    AMLVertexNode* AMLVertexArray[6];
    EdgeNode*      EdgeArray[10];
    AMLVertexArray[0] = LTT_Undigraph_Make_Vertex("A", NULL, 0);
    AMLVertexArray[1] = LTT_Undigraph_Make_Vertex("B", NULL, 0);
    AMLVertexArray[2] = LTT_Undigraph_Make_Vertex("C", NULL, 0);
    AMLVertexArray[3] = LTT_Undigraph_Make_Vertex("D", NULL, 0);
    AMLVertexArray[4] = LTT_Undigraph_Make_Vertex("E", NULL, 0);
    AMLVertexArray[5] = LTT_Undigraph_Make_Vertex("F", NULL, 0);

    int distence[10]  = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    EdgeArray[0]      = LTT_Undigraph_Make_Edge("A", "B", &distence[6], sizeof(int));
    EdgeArray[1]      = LTT_Undigraph_Make_Edge("A", "C", &distence[1], sizeof(int));
    EdgeArray[2]      = LTT_Undigraph_Make_Edge("A", "D", &distence[5], sizeof(int));
    EdgeArray[3]      = LTT_Undigraph_Make_Edge("B", "C", &distence[5], sizeof(int));
    EdgeArray[4]      = LTT_Undigraph_Make_Edge("B", "E", &distence[3], sizeof(int));
    EdgeArray[5]      = LTT_Undigraph_Make_Edge("C", "D", &distence[5], sizeof(int));
    EdgeArray[6]      = LTT_Undigraph_Make_Edge("C", "E", &distence[6], sizeof(int));
    EdgeArray[7]      = LTT_Undigraph_Make_Edge("C", "F", &distence[4], sizeof(int));
    EdgeArray[8]      = LTT_Undigraph_Make_Edge("D", "F", &distence[2], sizeof(int));
    EdgeArray[9]      = LTT_Undigraph_Make_Edge("E", "F", &distence[6], sizeof(int));

    G                 = LTT_Undigraph_New(AMLVertexArray, 6, EdgeArray, 10);

    // AMLUndigraph*  MST;
    // AMLVertexNode* FirstP = LTT_Undigraph_Vertex_Exist_byID_Safe(G, "A");
    // //MST                   = LTT_Undigraph_MiniSpanTree_Prim(G, FirstP);
    // MST                   = LTT_Undigraph_MiniSpanTree_Kruskal(G, FirstP, cmp);
    // LTT_Undigraph_DFS_Traverse_Recursive(MST, "A", VISIT);
    // printf("@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    // LTT_Undigraph_DFS_Traverse_Stack(MST, "A", VISIT);
    // printf("@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    // LTT_Undigraph_BFS_Traverse(MST, "A", VISIT);
    // printf("Test Over!\n");
    int** AdjMatrix   = LTT_Undigraph_Make_AdjMatrix(G);
    int   VertexNum   = LTT_Undigraph_Get_VertexNum(G);
    for (int i = 0; i <= VertexNum; ++i)
    {
        for (int j = 0; j <= VertexNum; ++j) { printf("%d ", AdjMatrix[i][j]); }
        printf("\n");
    }
}
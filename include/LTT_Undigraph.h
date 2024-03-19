#pragma once
#include "../src/Iterator/Iterator.h"
#include "../src/Predefined/Predefined.h"
#include "../src/Undigraph/Undigraph.h"


#ifdef __cplusplus
extern "C"
{
#endif
    //初始化一个无向图
    AMLUndigraph* LTT_Undigraph_New(AMLVertexNode** const AMLVertexArray, const int VertexNum, EdgeNode** const EdgeArray, const int EdgeNum);
    //清空一个无向图
    Status LTT_Undigraph_Clear(AMLUndigraph* const GP);
    //销毁一个无向图
    Status LTT_Undigraph_Destroy(AMLUndigraph* GP);
    //创建一个顶点
    AMLVertexNode* LTT_Undigraph_MakeVertex(const char* const VertexID, void* const Data, const size_t DataSize);
    //插入一个顶点
    Status LTT_Undigraph_InsertVertex(AMLUndigraph* const GP, AMLVertexNode* const VP);
    //删除一个顶点
    Status LTT_Undigraph_DeleteVertex_byID(AMLUndigraph* const GP, const char* const VertexID);
    Status LTT_Undigraph_Delete_Vertex_byVP(AMLUndigraph* const GP, AMLVertexNode* VP);
    //获取一个顶点的数据
    void* LTT_Undigraph_Get_Data_From_Vertex_byID(const AMLUndigraph* const GP, const char* const VertexID);
    void* LTT_Undigraph_Get_Data_From_Vertex_byVP(const AMLUndigraph* const GP, const AMLVertexNode* const VP);
    void* LTT_Undigraph_Get_Data_From_Vertex_byVP_Uncheck(const AMLVertexNode* const VP);
    // 获取一个顶点的ID
    const char* LTT_Undigraph_Get_VertexID_byVP_Uncheck(const AMLVertexNode* const VP);
    //修改一个顶点的数据
    Status LTT_Undigraph_Set_Data_From_Vertex_byID(const AMLUndigraph* const GP, const char* const VertexID, const void* const Data, const size_t DataSize);
    Status LTT_Undigraph_Set_Data_From_Vertex_byVP(const AMLUndigraph* const GP, AMLVertexNode* const VP, const void* const Data, const size_t DataSize);
    //判断顶点是否存在
    AMLVertexNode* LTT_Undigraph_Vertex_Exist_byID_Safe(const AMLUndigraph* const GP, const char* const VertexID);
    bool           LTT_Undigraph_Vertex_Exist_byVP(const AMLUndigraph* const GP, const AMLVertexNode* const VP);
    //创建一条边
    EdgeNode* LTT_Undigraph_Make_Edge(const char* const IvertexID, const char* const JvertexID, void* const Data, const size_t DataSize);
    //插入一条边
    Status LTT_Undigraph_Insert_Edge(AMLUndigraph* const GP, EdgeNode* const EP);
    //删除一条边
    Status LTT_Undigraph_Delete_Edge_byID(AMLUndigraph* const GP, const char* const IvertexID, const char* const JvertexID);
    Status LTT_Undigraph_Delete_Edge_byEP_Safe(AMLUndigraph* const GP, EdgeNode* EP);
    //获取一个边的数据
    void* LTT_Undigraph_Get_Data_From_Edge_byID(const AMLUndigraph* const GP, const char* IvertexID, const char* JvertexID);
    void* LTT_Undigraph_Get_Data_From_Edge_byEP(const AMLUndigraph* const GP, const EdgeNode* const EP);
    void* LTT_Undigraph_Get_Data_From_Edge_byEP_Uncheck(const EdgeNode* const EP);
    //修改一个边的数据
    Status LTT_Undigraph_Set_Data_From_Edge_byID(const AMLUndigraph* const GP, const char* const IvertexID, const char* const JvertexID, const void* Data, const size_t DataSize);
    Status LTT_Undigraph_Set_Data_From_Edge_byEP(const AMLUndigraph* const GP, EdgeNode* const EP, const void* Data, const size_t DataSize);
    //判断边是否存在
    EdgeNode* LTT_Undigraph_Edge_Exist_by_ID(const AMLUndigraph* const GP, const char* const IvertexID, const char* const JvertexID);
    bool      LTT_Undigraph_Edge_Exist_by_EP(const AMLUndigraph* const GP, const EdgeNode* const EP);
    //创建邻接矩阵
    Status LTT_Undigraph_CreateAdjMatrix_AML(const AMLUndigraph* const GP, int*** const AdjMatrix);
    //返回第一个邻接顶点
    AMLVertexNode* LTT_Undigraph_First_Adj_Vertex_byIP(const AMLUndigraph* const GP, const char* const VertexID);
    AMLVertexNode* LTT_Undigraph_First_AML_Vertex_byVP(const AMLUndigraph* const GP, const AMLVertexNode* const VP);
    //返回第一个未访问的邻接顶点
    AMLVertexNode* LTT_Undigraph_Frist_Unvisited_Adj_Vertex_byID(const AMLUndigraph* const GP, const char* const VertexID);
    AMLVertexNode* LTT_Undigraph_Frist_Unvisited_Adj_Vertex_byVP_Safe(const AMLUndigraph* const GP, const AMLVertexNode* const VP);
    //返回指向当前边的上一条边的指针的指针
    EdgeNode** LTT_Undigraph_Get_Prior_EdgePP_byID_Safe(const AMLUndigraph* const GP, const char* const VertexID, const char* const IvertexID, const char* const JvertexID);
    EdgeNode** LTT_Undigraph_Get_Prior_EdgePP_byVE_Safe(const AMLUndigraph* const GP, AMLVertexNode* const VP, const EdgeNode* const EP);
    //返回指向当前顶点开始的尾边的指针的指针
    EdgeNode** LTT_Undigraph_Get_Tail_EdgePP_by_ID_Safe(const AMLUndigraph* const GP, const char* const VertexID);
    EdgeNode** LTT_Undigraph_Get_Tail_EdgePP_by_VP_Safe(const AMLUndigraph* const GP, AMLVertexNode* const VP);
    //DFS遍历，使用递归
    Status LTT_Undigraph_DFS_Traverse_Recursive(const AMLUndigraph* const GP, const char* const VertexID, const VisitFunction Visit);
    //DFS遍历，使用栈
    Status LTT_Undigraph_DFS_Traverse_Stack(const AMLUndigraph* const GP, const char* const VertexID, const VisitFunction Visit);
    //BFS遍历，使用队列
    Status LTT_Undigraph_BFS_Traverse(const AMLUndigraph* const GP, const char* const VertexID, const VisitFunction Visit);
    //返回连通分量个数
    int LTT_Undigraph_Get_ConnectedComponentNumber(const AMLUndigraph* const GP);
    //判断是否有回路
    bool LTT_Undigraph_HasCircle(const AMLUndigraph* const GP);
    //最小生成树，使用Prim算法
    AMLUndigraph* LTT_Undigraph_MiniSpanTree_Prim(const AMLUndigraph* const GP, const AMLVertexNode* const VP);
    //最小生成树，使用Kruskal算法
    /*
        Comparator: 用于比较边权重的函数
        注意Comparator比较的是两个边的指针的指针
    */
    AMLUndigraph* LTT_Undigraph_MiniSpanTree_Kruskal(const AMLUndigraph* const GP, const AMLVertexNode* const VP, const CompareFunction Comparator);
    //最短路径，使用Dijkstra算法
    Status LTT_Undigraph_ShortestPath_Dijkstra(const AMLUndigraph* const GP, const int Vid);
    //最短路径，使用Floyd-Warshall算法
    Status LTT_Undigraph_ShortestPath_Floyd_Warshall(const AMLUndigraph* GP);
    //获取顶点迭代器
    Iterator LTT_Undigraph_GetIterator_Vertex(const AMLUndigraph* const GP);
    //获取边迭代器
    Iterator LTT_Undigraph_GetIterator_Edge(const AMLUndigraph* const GP);
    // 获取迭代器当前元素
    AMLVertexNode* LTT_Undigraph_GetCurrent_Vertex(const Iterator* const IP);
    EdgeNode*      LTT_Undigraph_GetCurrent_Edge(const Iterator* const IP);
#ifdef __cplusplus
}
#endif
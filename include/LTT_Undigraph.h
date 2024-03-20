#pragma once
#include "../src/Iterator/Iterator.h"
#include "../src/Predefined/Predefined.h"
#include "../src/Undigraph/Undigraph.h"

/*
    要创建一个无向图，先LTT_Undigraph_New初始化
    然后可以用LTT_Undigraph_Insert_VertexS插入一组顶点
    用LTT_Undigraph_Insert_EdgeS插入一组边
    必须要注意的是，插入边的时候，边的两个顶点必须要先插入
*/


#ifdef __cplusplus
extern "C"
{
#endif
    //初始化一个无向图
    AMLUndigraph* LTT_Undigraph_New(void);
    //清空一个无向图
    Status LTT_Undigraph_Clear(AMLUndigraph* const GP);
    //销毁一个无向图
    Status LTT_Undigraph_Destroy(AMLUndigraph* GP);
    //创建一个顶点
    AMLVertexNode* LTT_Undigraph_Make_Vertex(const char* const VertexID, void* const Data, const size_t DataSize);
    //插入一个顶点
    Status LTT_Undigraph_Insert_Vertex(AMLUndigraph* const GP, AMLVertexNode* const VP);
    Status LTT_Undigraph_Insert_VertexS(AMLUndigraph* const GP, AMLVertexNode** const AMLVertexArray, const int VertexNum);
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
    EdgeNode* LTT_Undigraph_Make_Edge_byVP_Safe(const AMLUndigraph* const GP, AMLVertexNode* const VIP, AMLVertexNode* const VJP, void* const Data, const size_t DataSize);
    EdgeNode* LTT_Undigraph_Make_Edge_byID(const AMLUndigraph* const GP, const char* const IvertexID, const char* const JvertexID, void* const Data, const size_t DataSize);
    //插入一条边
    Status LTT_Undigraph_Insert_Edge(AMLUndigraph* const GP, EdgeNode* const EP);
    Status LTT_Undigraph_Insert_EdgeS(AMLUndigraph* const GP, EdgeNode** const EdgeArray, const int EdgeNum);
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
    //获得顶点个数
    int LTT_Undigraph_Get_VertexNum(const AMLUndigraph* const GP);
    //获得边的个数
    int LTT_Undigraph_Get_EdgeNum(const AMLUndigraph* const GP);
    //创建邻接矩阵
    int** LTT_Undigraph_Make_AdjMatrix(const AMLUndigraph* const GP);
    /*  
        创建带权邻接矩阵
        权重只能是int型变量,且放在Edge.Data里面
    */
    int** LTT_Undigraph_Make_Weighted_AdjMatrix(const AMLUndigraph* const GP);
    //返回第一个邻接顶点
    AMLVertexNode* LTT_Undigraph_First_Adj_Vertex_byID(const AMLUndigraph* const GP, const char* const VertexID);
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
    //返回顶点的索引值
    int LTT_Undigraph_Get_VertexIndex_by_VP(const AMLUndigraph* const GP, const AMLVertexNode* const VP);
    int LTT_Undigraph_Get_VertexIndex_by_ID(const AMLUndigraph* const GP, const char* const VertexID);

    //最短路径，使用Dijkstra算法
    Status LTT_Undigraph_ShortestPath_Dijkstra(const AMLUndigraph* const GP, const char* const VertexID, int** DisMatrix, int** Path);
    /*
        最短路径，使用Floyd-Warshall算法
        传入两个int型二维矩阵,用来存放最短距离以及路径
        具体规则为DisMatrix[i][j]即为顶点i到顶点j的最短路径的值
        Path[i][j]为从顶点j到顶点iu必须要经过的顶点的索引,直到Path[i][i]
        同时会打印每一个顶点对应的索引值以方便查找
    */
    Status LTT_Undigraph_ShortestPath_Floyd_Warshall(const AMLUndigraph* const GP, int*** DisMatrix, int*** Path);
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
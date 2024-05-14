#pragma once
/*
    要创建一个无向图，先LTT_Undigraph_New初始化
    然后可以用LTT_Undigraph_Insert_VertexS插入一组顶点
    用LTT_Undigraph_Insert_EdgeS插入一组边
    必须要注意的是，插入边的时候，边的两个顶点必须要先插入
*/

#include "../src/Undigraph/Undigraph.h"    // IWYU pragma: export
#pragma once

typedef enum
{
    Unvisited,
    Visited
} VisitIf;

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _EdgeNode      EdgeNode;
    typedef struct _AMLVertexNode AMLVertexNode;
    typedef struct _AMLUndigraph  AMLUndigraph;
#ifdef __cplusplus
}
#endif

#pragma once

#define VID_MAX_LENGTH 1 << 5
#define EID_MAX_LENGTH 1 << 6
#define MAX_VERTEX_NUM 100

typedef enum
{
    Unvisited,
    Visited
} VisitIf;
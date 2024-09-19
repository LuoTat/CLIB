#pragma once
#include "..\Predefined\Predefined.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    char* String;
    int   Length;
} SqString;

void   InitString(SqString* SqStringPointer);
Status StrAssign(SqString* SqStringPointer, const char* Chars);
int    StrLength(SqString SqString);
int    StrCompare(SqString SqString1, SqString SqString2);
Status ClearString(SqString* SqString);
Status StrConcat(SqString* SqStringConcated, SqString SqString1, SqString SqString2);
Status SubString(SqString* SubSqString, SqString SqString, int Pos, int Len);
int    Index(SqString MainSqString, SqString PatternSqString, int Pos);
int    IndexKMP(SqString MainSqString, SqString PatternSqString, int Pos);
void   GetNextVal(SqString PatternSqString, int* NextVal);
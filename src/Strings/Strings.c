#include "Strings.h"

void InitString(SqString* SqStringPointer)
{
    SqStringPointer->String = NULL;
    SqStringPointer->Length = 0;
}

Status StrAssign(SqString* SqStringPointer, const char* Chars)
{
    if (SqStringPointer->String) free(SqStringPointer->String);
    int         Length = 0;
    const char* c      = Chars;
    for (Length, c; *c; ++Length, ++c);
    if (!Length)
    {
        SqStringPointer->String = NULL;
        SqStringPointer->Length = 0;
    }
    else
    {
        if (!(SqStringPointer->String = (char*)malloc((Length + 1) * sizeof(char)))) exit(OVERFLOW);
        for (int i = 0; i < Length; i++) SqStringPointer->String[i] = Chars[i];
        SqStringPointer->String[Length] = '\0';
        SqStringPointer->Length         = Length;
    }
    return OK;
}

int StrLength(SqString SqString) { return SqString.Length; }

int StrCompare(SqString SqString1, SqString SqString2)
{
    for (int i = 0; i < SqString1.Length && i < SqString2.Length; i++)
    {
        if (SqString1.String[i] != SqString2.String[i]) return SqString1.String[i] - SqString2.String[i];
    }
    return SqString1.Length - SqString2.Length;
}

Status ClearString(SqString* SqString)
{
    if (SqString->String)
    {
        free(SqString->String);
        SqString->String = NULL;
    }
    SqString->Length = 0;
    return OK;
}

Status StrConcat(SqString* SqStringConcated, SqString SqString1, SqString SqString2)
{
    if (SqStringConcated->String) free(SqStringConcated->String);
    if (!(SqStringConcated->String = (char*)malloc((SqString1.Length + SqString2.Length + 1) * sizeof(char)))) exit(OVERFLOW);
    for (int i = 0; i < SqString1.Length; i++) SqStringConcated->String[i] = SqString1.String[i];
    for (int i = SqString1.Length; i < SqString1.Length + SqString2.Length; i++) SqStringConcated->String[i] = SqString2.String[i - SqString1.Length];
    SqStringConcated->String[SqString1.Length + SqString2.Length] = '\0';
    SqStringConcated->Length                                      = SqString1.Length + SqString2.Length;
    return OK;
}

Status SubString(SqString* SubSqString, SqString SqString, int Pos, int Len)
{
    if (Pos < 1 || Pos > SqString.Length || Len < 0 || Len > SqString.Length - Pos + 1) return ERROR;
    if (SubSqString->String) free(SubSqString->String);
    if (!Len)
    {
        SubSqString->String = NULL;
        SubSqString->Length = 0;
    }
    else
    {
        if (!(SubSqString->String = (char*)malloc((Len + 1) * sizeof(char)))) exit(OVERFLOW);
        for (int i = 0; i < Len; i++) SubSqString->String[i] = SqString.String[i + Pos - 1];
        SubSqString->String[Len] = '\0';
        SubSqString->Length      = Len;
    }
    return OK;
}

int Index(SqString MainSqString, SqString PatternSqString, int Pos)
{
    int MainSqStringPos = Pos, PatternSqStringIndex = 0;
    while (MainSqStringPos <= MainSqString.Length && PatternSqStringIndex < PatternSqString.Length)
    {
        if (MainSqString.String[MainSqStringPos - 1] == PatternSqString.String[PatternSqStringIndex])
        {
            ++MainSqStringPos;
            ++PatternSqStringIndex;
        }
        else
        {
            MainSqStringPos      -= PatternSqStringIndex - 1;
            PatternSqStringIndex  = 0;
        }
    }
    if (PatternSqStringIndex >= PatternSqString.Length) return MainSqStringPos - PatternSqString.Length;
    else return 0;
}

int IndexKMP(SqString MainSqString, SqString PatternSqString, int Pos)
{
    int  MainSqStringPos = Pos, PatternSqStringIndex = 0;
    int* NextVal;
    if (!(NextVal = (int*)malloc(PatternSqString.Length * sizeof(int)))) exit(OVERFLOW);
    GetNextVal(PatternSqString, NextVal);
    while (MainSqStringPos <= MainSqString.Length && PatternSqStringIndex < PatternSqString.Length)
    {
        if (PatternSqStringIndex == 0 || MainSqString.String[MainSqStringPos - 1] == PatternSqString.String[PatternSqStringIndex])
        {
            ++MainSqStringPos;
            ++PatternSqStringIndex;
        }
        else PatternSqStringIndex = NextVal[PatternSqStringIndex];
    }
    if (PatternSqStringIndex >= PatternSqString.Length)
    {
        free(NextVal);
        return MainSqStringPos - PatternSqString.Length;
    }
    else
    {
        free(NextVal);
        return 0;
    }
}

void GetNextVal(SqString PatternSqString, int* NextVal)
{
    NextVal[0] = 0;
    int Index  = 1;
    int Now    = 0;
    while (Index < PatternSqString.Length)
    {
        if (PatternSqString.String[Now] == PatternSqString.String[Index])
        {
            ++Now;
            ++Index;
            NextVal[Index - 1] = Now;
        }
        else if (Now) Now = NextVal[Now - 1];
        else
        {
            NextVal[Index] = 0;
            Index++;
        }
    }
}
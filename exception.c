//=============================================================================
//                       ___
//  __         __   __  |   
// |   \/ /|  |__  |  | |__
// |__ /\ _|_ |__| |__|    |
// ________________________|
// Commander X16 Graphical Multitasking Environment
//
// File:        exception.c
//=============================================================================

//========================================
// System Includes
//========================================

#include <stdio.h>

//========================================
// Project Includes
//========================================

#include "cx16os.h"

void FAR Assert(BOOL bCondition, LPCSTR lpFile, LPCSTR lpFunction, WORD lpLine, LPCSTR lpErrorMessage)
{
    if(!bCondition)
    {
        printf("Assertion failed in %s\n%s:%d %s\n", lpFile, lpFunction, lpLine, lpErrorMessage);
        while(1);
    }
}
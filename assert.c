#include <stdio.h>

#include "cx16os.h"

void FAR Assert(BOOL bCondition, LPCSTR lpFile, LPCSTR lpFunction, WORD lpLine, LPCSTR lpErrorMessage)
{
    if(!bCondition)
    {
        printf("Assertion failed in %s\n%s:%d %s\n", lpFile, lpFunction, lpLine, lpErrorMessage);
        while(1);
    }
}
#include <stdlib.h>
#include <stdio.h>

#include "cx16os.h"

void FAR RegisterGlobalSymbol(LPCSTR lpSymbolName, LPVOID lpSymbolAddress)
{

}

void FAR RemoveGlobalSymbol(LPCSTR lpSymbolName)
{

}

LPVOID LookupGlobalSymbol(LPCSTR lpSymbolName)
{
    return NULL;
}

typedef struct MODULE_ {
    
} MODULE;
typedef MODULE        *PMODULE;
typedef MODULE NEAR   *NPMODULE;
typedef MODULE FAR    *LPMODULE;

HANDLE FAR LoadModule(LPCSTR lpModuleName)
{
    return INVALID_HANDLE;
}

BOOL FAR FreeModule(HANDLE hModule)
{
    return FALSE;
}

LPVOID FAR GetProcAddress(HANDLE hModule, LPCSTR lpName)
{
    return NULL;
}
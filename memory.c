#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cx16os.h"

#define MAX_HANDLES 256
#define HANDLE_ALLOCATED (1 << 0)
#define HANDLE_LOCKED (1 << 1)

typedef struct HANDLEENTRY_ {
    BOOL Locked;
    LPVOID Memory;
} HANDLEENTRY;
typedef HANDLEENTRY      *PHANDLEENTRY;
typedef HANDLEENTRY NEAR *NPHANDLEENTRY;
typedef HANDLEENTRY FAR  *LPHANDLEENTRY;

struct {
    HANDLEENTRY Handles[MAX_HANDLES];
    WORD Head;
} HandleHeap;

void FAR InitializeMemory()
{
    __set_heap_limit(16384);
    printf("memory: increased heap limit to %u bytes\n", __heap_limit());
    printf("memory: %u/%u bytes (%u bytes free)\n", __heap_bytes_used(), __heap_limit(), __heap_bytes_free());
    printf("memory: %u handles allocated\n", MAX_HANDLES);
    memset(&HandleHeap, 0, sizeof(HandleHeap));
}

LPVOID FAR AllocateMemory(WORD wSize)
{
    LPVOID lpMemory = malloc(wSize);
    ASSERT(lpMemory != NULL);
    return lpMemory;
}

void FAR FreeMemory(LPVOID lpMemory)
{
    ASSERT(lpMemory != NULL);
    free(lpMemory);
}

LPHANDLEENTRY FAR GetFreeHandle(LPHANDLE lpHandle)
{
    LPHANDLEENTRY lpEntry;
    for(HANDLE hHandle = HandleHeap.Head; hHandle < MAX_HANDLES; hHandle++)
    {
        lpEntry = &HandleHeap.Handles[hHandle];
        if(lpEntry->Memory == NULL)
        {
            *lpHandle = hHandle;
            HandleHeap.Head = hHandle;
            return lpEntry;
        }
    }
    for(HANDLE hHandle = 0; hHandle < HandleHeap.Head; hHandle++)
    {
        lpEntry = &HandleHeap.Handles[hHandle];
        if(lpEntry->Memory == NULL)
        {
            *lpHandle = hHandle;
            HandleHeap.Head = hHandle;
            return lpEntry;
        }
    }
    ASSERT(!"out of handles");
    return NULL;
}

HANDLE FAR AllocateHandle(WORD wSize)
{
    HANDLE hHandle;
    LPHANDLEENTRY lpEntry = GetFreeHandle(&hHandle);
    lpEntry->Locked = 0;
    lpEntry->Memory = AllocateMemory(wSize);
    return hHandle;
}

void FAR FreeHandle(HANDLE hHandle)
{
    LPHANDLEENTRY lpEntry = &HandleHeap.Handles[hHandle];
    ASSERT(lpEntry->Locked == FALSE);
    ASSERT(lpEntry->Memory != NULL);
    FreeMemory(lpEntry->Memory);
    lpEntry->Memory = NULL;
}

LPVOID FAR LockHandle(HANDLE hHandle)
{
    LPHANDLEENTRY lpEntry = &HandleHeap.Handles[hHandle];
    ASSERT(lpEntry->Locked == FALSE);
    ASSERT(lpEntry->Memory != NULL);
    lpEntry->Locked = TRUE;
    return lpEntry->Memory;
}

void FAR UnlockHandle(HANDLE hHandle)
{
    LPHANDLEENTRY lpEntry = &HandleHeap.Handles[hHandle];
    ASSERT(lpEntry->Locked == TRUE);
    ASSERT(lpEntry->Memory != NULL);
    lpEntry->Locked = FALSE;
}
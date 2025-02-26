#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cx16os.h"

/* memory */

#define HANDLE_ALLOCATED 0x2000
#define HANDLE_LOCKED 0x4000
#define HANDLE_PAGED 0x8000

// Handle structure
typedef struct HANDLE_ENTRY_ {
    LPBYTE Pointer;
    WORD Size;
    WORD Flags;
} HANDLE_ENTRY;
typedef HANDLE_ENTRY NEAR  *PHANDLE_ENTRY;
typedef HANDLE_ENTRY FAR   *LPHANDLE_ENTRY;

// Memory allocator structure
#define MAX_HANDLES 256
typedef struct MEMORY_ALLOCATOR_ {
    LPBYTE HeapStart, HeapEnd, HeapHead;
    DWORD HeapSize;
    HANDLE_ENTRY Handles[MAX_HANDLES];
    LPHANDLE_ENTRY HandlesSorted[MAX_HANDLES];
} MEMORY_ALLOCATOR;
typedef MEMORY_ALLOCATOR NEAR  *PMEMORY_ALLOCATOR;
typedef MEMORY_ALLOCATOR FAR   *LPMEMORY_ALLOCATOR;

extern char __heap_start[];
extern char __stack[];

// Global allocator instance
static MEMORY_ALLOCATOR allocator;

// Helper function to validate handle
static BOOL IsValidHandle(HGLOBAL hMem) {
    return (hMem < MAX_HANDLES && (allocator.Handles[hMem].Flags & HANDLE_ALLOCATED));
}

// Function prototypes
void InitializeMemory(DWORD capacity) {
    allocator.HeapSize = capacity;
    allocator.HeapStart = (LPBYTE) __heap_start;
    allocator.HeapEnd = allocator.HeapStart + allocator.HeapSize;
    allocator.HeapHead = allocator.HeapStart;
    printf("Heap Setup [%p]-[%p] (%lu Bytes)\n", allocator.HeapStart, allocator.HeapEnd, allocator.HeapSize);
    memset(allocator.Handles, 0, sizeof(allocator.Handles));
}

HGLOBAL FAR GlobalAlloc(WORD wFlags, DWORD dwSize) {
    LPHANDLE_ENTRY entry = NULL;
    for (WORD i = 0; i < MAX_HANDLES; i++) {
        if((entry = &allocator.Handles[i])->Pointer == NULL) break;
    }
    if(entry == NULL)
    {
        ERROR("No free handles");
        return INVALID_HANDLE;
    }
    if (allocator.HeapHead + dwSize >= allocator.HeapEnd)
    {
        GlobalCompact(0);
        if (allocator.HeapHead + dwSize >= allocator.HeapEnd) ERROR("Out of memory");
    }
    entry->Pointer = allocator.HeapHead;
    entry->Size = dwSize;
    entry->Flags = wFlags | HANDLE_ALLOCATED;
    allocator.HeapHead += dwSize;
    if (wFlags & GMEM_ZEROINIT) memset(entry->Pointer, 0, entry->Size);
    printf("Allocated Memory: %p %lu Bytes\n", entry->Pointer, entry->Size);
    return entry - allocator.Handles;
}

LPVOID FAR GlobalLock(HGLOBAL hMem) {
    if (!IsValidHandle(hMem)) {
        ERROR("Invalid handle");
        return NULL;
    }
    LPHANDLE_ENTRY entry = &allocator.Handles[hMem];
    entry->Flags |= HANDLE_LOCKED;
    return entry->Pointer;
}

BOOL FAR GlobalUnlock(HGLOBAL hMem) {
    if (!IsValidHandle(hMem)) {
        ERROR("Invalid handle");
        return FALSE;
    }
    LPHANDLE_ENTRY entry = &allocator.Handles[hMem];
    entry->Flags &= ~HANDLE_LOCKED;
    return TRUE;
}

void FAR GlobalFree(HGLOBAL hMem) {
    if (!IsValidHandle(hMem)) {
        ERROR("Invalid handle");
        return;
    }
    LPHANDLE_ENTRY entry = &allocator.Handles[hMem];
    entry->Flags &= ~HANDLE_ALLOCATED;
}

DWORD FAR GlobalSize(HGLOBAL hMem) {
    if (!IsValidHandle(hMem)) {
        ERROR("Invalid handle");
        return 0;
    }
    LPHANDLE_ENTRY entry = &allocator.Handles[hMem];
    return entry->Size;
}

WORD FAR GlobalFlags(HGLOBAL hMem) {
    if (!IsValidHandle(hMem)) {
        ERROR("Invalid handle");
        return 0;
    }
    LPHANDLE_ENTRY entry = &allocator.Handles[hMem];
    return entry->Flags;
}

char print_display[256];

int CompareHandle(LPHANDLE_ENTRY handle1, LPHANDLE_ENTRY handle2)
{
    if ( handle1->Pointer == handle2->Pointer ) 
    {
        ERROR("unrecoverable memory corruption\n");
        return 0;
    }
    if ( handle1->Pointer < handle2->Pointer ) return -1;
    else return 1;
}

void FAR SortHandleTable()
{
    qsort(allocator.HandlesSorted)
}

BOOL FAR GlobalCompact(WORD wFlags) 
{
    LPHANDLE_ENTRY lowestHandle = NULL;
    for(WORD i = 0; i < MAX_HANDLES; i++)
    {
        LPHANDLE_ENTRY entry = &allocator.Handles[i];
        if(lowestHandle == NULL) lowestHandle = entry;
        if(entry->Pointer < lowestHandle->Pointer) lowestHandle = entry;
    }
}

void FAR GlobalHint(HGLOBAL hMem, WORD wFlags)
{
    
}
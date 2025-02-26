#include <stdlib.h>
#include <stdio.h>

#include "cx16os.h"

void InitializeMemory(DWORD capacity);
void FAR InitializeThreading();
INT FAR Scheduler();

INT main() {
    InitializeMemory(16384);

    // critical fixed low-memory (e.g for assembly libraries or drivers)
    HGLOBAL hFixed = GlobalAlloc(GMEM_ZEROINIT, 1024);
    // fixed pointers can be cached
    LPVOID lpFixed = GlobalLock(hFixed);

    HGLOBAL mem1 = GlobalAlloc(GMEM_ZEROINIT, 1234);
    HGLOBAL mem2 = GlobalAlloc(GMEM_ZEROINIT, 3234);
    GlobalFree(mem1);
    HGLOBAL mem3 = GlobalAlloc(GMEM_ZEROINIT, 2434);
    GlobalFree(mem2);
    HGLOBAL mem4 = GlobalAlloc(GMEM_ZEROINIT, 234);
    GlobalFree(mem3);
    GlobalFree(mem4);

    GlobalCompact(0);
    GlobalCompact(0);
    GlobalCompact(0);
    GlobalCompact(0);

    // general purpose memory
    HGLOBAL hPageable = GlobalAlloc(GMEM_PAGEABLE | GMEM_BANKABLE | GMEM_MOVEABLE | GMEM_ZEROINIT, 8192);
    {
        // may decompress or load from disk if needed
        LPVOID lpPageable = GlobalLock(hPageable);
        // do stuff
        // give control back to the system
        GlobalUnlock(hPageable);
        // speed is not critical, compress or store to disk if needed
        // these are only hints, the system does compression and paging automatically if resources are exhausted
        GlobalHint(hPageable, GHINT_COMPRESS | GHINT_PAGEABLE);
    }

    return 0;
}
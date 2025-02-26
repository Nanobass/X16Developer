#pragma once

#define FALSE 0
#define TRUE  1

#define NEAR
#define FAR

#define INVALID_HANDLE 0xFFFF

#define CHECK_MAX(X,MAX) if(X>=MAX)
#define CHECK_MIN(X,MIN) if(X<MIN)
#define CHECK_RANGE(X,MIN,MAX) if(X<MIN&&X>=MAX)
#define CHECK_ERROR(X) if(X<0)
#define CHECK_NONZERO(X) if(!X)
#define ERROR_RETURN(VALUE,FORMAT,...) { printf(FORMAT, __VA_ARGS__); return VALUE; }

typedef char              BOOL;
typedef unsigned char     BYTE;
typedef char              STR;
typedef char              CHAR;
typedef unsigned int      WORD;
typedef int               INT;
typedef unsigned long     DWORD;
typedef long              LONG;
typedef STR NEAR         *PSTR;
typedef STR FAR          *LPSTR;
typedef STR const NEAR   *PCSTR;
typedef STR const FAR    *LPCSTR;
typedef BYTE NEAR        *PBYTE;
typedef BYTE FAR         *LPBYTE;
typedef INT NEAR         *PINT;
typedef INT FAR          *LPINT;
typedef WORD NEAR        *PWORD;
typedef WORD FAR         *LPWORD;
typedef LONG NEAR        *PLONG;
typedef LONG FAR         *LPLONG;
typedef DWORD NEAR       *PDWORD;
typedef DWORD FAR        *LPDWORD;
typedef void NEAR        *PVOID;
typedef void FAR         *LPVOID;
typedef WORD              HANDLE;
typedef HANDLE NEAR      *SPHANDLE;
typedef HANDLE FAR       *LPHANDLE;

typedef INT (FAR  *FARPROC)();
typedef INT (NEAR *NEARPROC)();

typedef HANDLE HMODULE;
typedef HANDLE HGLOBAL;
typedef HANDLE HSTR;

/*
*   Memory API
*/

enum GMEM {
    GMEM_MOVEABLE    = 0x0001,
    GMEM_PAGEABLE    = 0x0002,
    GMEM_BANKABLE    = 0x0004,
    GMEM_ZEROINIT    = 0x0008,
};

enum GHINT {
    GHINT_PAGEABLE   = 0x0001,
    GHINT_COMPRESS   = 0x0002,
};

BOOL FAR GlobalCompact(WORD wFlags);
void FAR GlobalHint(HGLOBAL hMem, WORD wFlags);
HGLOBAL FAR GlobalAlloc(WORD wFlags, DWORD dwSize);
HGLOBAL FAR GlobalReAlloc(HGLOBAL hMem, DWORD dwBytes, WORD wFlags);
void FAR GlobalFree(HGLOBAL hMem);
DWORD FAR GlobalSize(HGLOBAL hMem);
WORD FAR GlobalFlags(HGLOBAL hMem);
LPVOID FAR GlobalLock(HGLOBAL hMem);
BOOL FAR GlobalUnlock(HGLOBAL hMem);

/*
*   Threading API
*/

typedef struct CONTEXT_ {
    LPVOID ReturnAddress;
    BYTE HardStackPointer;
    LPVOID SoftStackPointer;
    BYTE CalleeSavedRegisters[14];
} CONTEXT;
typedef CONTEXT NEAR *PCONTEXT;
typedef CONTEXT FAR  *LPCONTEXT;

WORD FAR SaveContextAndStack(LPCONTEXT lpContext, LPVOID lpHardStack);
void FAR LoadContextAndStack(LPCONTEXT lpContext, LPVOID lpHardStack);
WORD FAR SaveContext(LPCONTEXT lpContext);
void FAR LoadContext(LPCONTEXT lpContext, WORD wParam);

typedef struct THREAD_ {
    struct THREAD_ FAR *Prev;
    struct THREAD_ FAR *Next;

    STR Name[16];
    WORD Status;
    CONTEXT Context;

    void (FAR *Entry) (LPVOID);
    LPVOID Args;
    
    BYTE HardStack[256];
    WORD StackSize;
    BYTE SoftStack[];
} THREAD;
typedef THREAD NEAR *PTHREAD;
typedef THREAD FAR  *LPTHREAD;

LPTHREAD FAR CreateThread(void (FAR *lpEntry) (LPVOID), LPVOID lpArgs, WORD wStackSize, LPCSTR lpName);
void     FAR SleepThread(WORD wMillis);
void     FAR KillThread(LPTHREAD hThread);
void     FAR YieldThread();
LPTHREAD FAR CurrentThread();
LPCSTR   FAR GetThreadName(LPTHREAD lpThread);

/*
*   Exception API
*/

#define ASSERT(expression) Assert(expression, __FILE__, __PRETTY_FUNCTION__, __LINE__, #expression)
#define ERROR(message) Assert(0, __FILE__, __PRETTY_FUNCTION__, __LINE__, #message)
#define TRY CONTEXT wExceptionContext[1]; WORD wExceptionParam = SaveContext(wExceptionContext); if(wExceptionParam == 0)
#define CATCH(wExceptionParam) else
#define THROW(wParam) LoadContext(wExceptionContext, wParam)

void FAR Assert(BOOL bCondition, LPCSTR lpFile, LPCSTR lpFunction, WORD lpLine, LPCSTR lpErrorMessage);

/*
*   Kernel API
*/

HMODULE FAR LoadModule(LPCSTR lpModuleName);
BOOL    FAR FreeModule(HMODULE hModule);
LPVOID  FAR GetProcAddress(HMODULE hModule, LPCSTR lpName);
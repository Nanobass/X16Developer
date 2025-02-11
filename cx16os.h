#pragma once

#define FALSE 0
#define TRUE  1

#define NEAR
#define FAR

#define INVALID_HANDLE 0xFFFF

typedef char              BOOL;
typedef unsigned char     BYTE;
typedef char              STR;
typedef unsigned int      WORD;
typedef int               INT;
typedef unsigned long     DWORD;
typedef long              LONG;
typedef char NEAR        *PSTR;
typedef char FAR         *LPSTR;
typedef char const NEAR  *PCSTR;
typedef char const FAR   *LPCSTR;
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

/*
*   Memory API
*/

LPVOID FAR AllocateMemory(WORD wSize);
void   FAR FreeMemory(LPVOID lpMemory);
HANDLE FAR AllocateHandle(WORD wSize);
BOOL   FAR ReallocateHandle(HANDLE hHandle, WORD wSize);
void   FAR FreeHandle(HANDLE hHandle);
LPVOID FAR LockHandle(HANDLE hHandle);
void   FAR UnlockHandle(HANDLE hHandle);

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

WORD   FAR GetVersion(void);
WORD   FAR GetNumTasks(void);

HANDLE FAR LoadModule(LPCSTR lpModuleName);
BOOL   FAR FreeModule(HANDLE hModule);
LPVOID FAR GetProcAddress(HANDLE hModule, LPCSTR lpName);
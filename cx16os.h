#pragma once

#define FALSE		    0
#define TRUE		    1

#define NEAR
#define FAR

#define MAKELONG(a, b)	    ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)	    ((WORD)(l))
#define HIWORD(l)	    ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)	    ((BYTE)(w))
#define HIBYTE(w)	    ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#define BITTEST(w, m)   (w & m)

typedef char	          BOOL;
typedef unsigned char	  BYTE;
typedef unsigned int	  WORD;
typedef unsigned long	  DWORD;
typedef char	          STR;
typedef char NEAR	     *PSTR;
typedef char NEAR	     *NPSTR;
typedef char FAR	     *LPSTR;
typedef char const NEAR  *PCSTR;
typedef char const NEAR  *NPCSTR;
typedef char const FAR   *LPCSTR;
typedef BYTE NEAR	     *PBYTE;
typedef BYTE FAR	     *LPBYTE;
typedef int NEAR	     *PINT;
typedef int FAR 	     *LPINT;
typedef WORD NEAR	     *PWORD;
typedef WORD FAR	     *LPWORD;
typedef long NEAR	     *PLONG;
typedef long FAR	     *LPLONG;
typedef DWORD NEAR	     *PDWORD;
typedef DWORD FAR	     *LPDWORD;
typedef void FAR	     *LPVOID;
typedef WORD		      HANDLE;
typedef HANDLE		     *PHANDLE;
typedef HANDLE NEAR	     *SPHANDLE;
typedef HANDLE FAR	     *LPHANDLE;

#define INVALID_HANDLE 0xFFFF

/*
*   Assertion API
*/

#define ASSERT(expression) Assert(expression, __FILE__, __PRETTY_FUNCTION__, __LINE__, #expression)

void FAR Assert(BOOL bCondition, LPCSTR lpFile, LPCSTR lpFunction, WORD lpLine, LPCSTR lpErrorMessage);

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

typedef struct THREAD_ THREAD;
typedef THREAD        *PTHREAD;
typedef THREAD NEAR   *NPTHREAD;
typedef THREAD FAR    *LPTHREAD;

LPTHREAD FAR CreateThread(void (FAR *lpEntry) (LPVOID), LPVOID lpArgs, WORD wStackSize, LPCSTR lpName);
void     FAR SleepThread(WORD wMillis);
void     FAR KillThread(LPTHREAD hThread);
void     FAR YieldThread();
LPTHREAD FAR CurrentThread();
LPCSTR   FAR GetThreadName(LPTHREAD lpThread);

/*
*   Kernel API
*/

WORD   FAR GetVersion(void);
WORD   FAR GetNumTasks(void);

HANDLE FAR LoadModule(LPCSTR lpModuleName);
BOOL   FAR FreeModule(HANDLE hModule);
LPVOID FAR GetProcAddress(HANDLE hModule, LPCSTR lpName);
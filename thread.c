//=============================================================================
//                       ___
//  __         __   __  |   
// |   \/ /|  |__  |  | |__
// |__ /\ _|_ |__| |__|    |
// ________________________|
// Commander X16 Graphical Multitasking Environment
//
// File:        thread.c
//
// Description: Cooperative Threading implementation using Context Switching.
//              because the 6502 and 65C02 can't relocate the hardware stack
//              the context switch will store the hardware stack in the threads 
//              context and restore it again when needed.
//              the scheduler will exit when all threads have exited
//=============================================================================

//========================================
// System Includes
//========================================

#include <stdio.h>
#include <stdlib.h>

//========================================
// Project Includes
//========================================

#include "cx16os.h"

static CONTEXT SchedulerContext, SchedulerExit;
static BYTE SchedularHardStack[256];
static LPTHREAD Current, ThreadToDelete;

/*
*   Initialize Threading System and Scheduler
*/

void FAR InitializeThreading()
{
    Current = NULL;
    ThreadToDelete = NULL;
}

int FAR Scheduler()
{
    if(SaveContextAndStack(&SchedulerExit, SchedularHardStack) == 0)
    {
        SaveContextAndStack(&SchedulerContext, SchedularHardStack);
        if(ThreadToDelete)
        {
            FreeMemory(ThreadToDelete);
            ThreadToDelete = NULL;
        }
        if(Current == NULL) LoadContextAndStack(&SchedulerExit, SchedularHardStack);
        Current = Current->Next;
        LoadContextAndStack(&Current->Context, Current->HardStack);
    }
    return 0;
}

/*
*   Safety Thread Wrapper
*/

LPTHREAD FAR ThreadExit()
{
    LPTHREAD lpPrevThread = Current->Prev;
    LPTHREAD lpNextThread = Current->Next;
    LPTHREAD lpToDelete = Current;
    if (Current == lpPrevThread) {
        Current = NULL;
    } else {
        lpPrevThread->Next = lpNextThread;
        lpNextThread->Prev = lpPrevThread;
        Current = lpNextThread;
    }
    return lpToDelete;
}

void FAR ThreadWrap()
{
    Current->Entry(Current->Args);
    ThreadToDelete = ThreadExit();
    LoadContextAndStack(&SchedulerContext, SchedularHardStack);
}

/*
*   API implementation
*/

LPTHREAD FAR CreateThread(void (FAR *lpEntry) (LPVOID), LPVOID lpArgs, WORD wStackSize, LPCSTR lpName)
{
    LPTHREAD lpThread = (LPTHREAD) AllocateMemory(sizeof(THREAD) + wStackSize);
    strncpy(lpThread->Name, lpName, sizeof(lpThread->Name));
    lpThread->Status = 0;
    lpThread->StackSize = wStackSize;
    lpThread->Entry = lpEntry;
    lpThread->Args = lpArgs;

    lpThread->Context.ReturnAddress = (char*) &ThreadWrap - 1;
    lpThread->Context.HardStackPointer = 0xFD;
    lpThread->Context.SoftStackPointer = (LPVOID) lpThread->SoftStack + wStackSize;

    if (!Current) {
        Current = lpThread;
        lpThread->Next = lpThread;
        lpThread->Prev = lpThread;
    } else {
        LPTHREAD lpNextThread = Current->Next;
        Current->Next = lpThread;
        lpThread->Prev = Current;
        lpThread->Next = lpNextThread;
        lpNextThread->Prev = lpThread;
    }

    return lpThread;
}

void FAR KillThread(LPTHREAD lpThread)
{
    if(lpThread == Current)
    {
        ThreadToDelete = ThreadExit();
        LoadContextAndStack(&SchedulerContext, SchedularHardStack);
    }
    ERROR("not implemented");
}

void FAR SleepThread(WORD wMillis)
{
    ERROR("not implemented");
}

void FAR YieldThread()
{
    if(SaveContextAndStack(&Current->Context, Current->HardStack) == 0) LoadContextAndStack(&SchedulerContext, SchedularHardStack);
}

LPTHREAD FAR CurrentThread()
{
    return Current;
}

LPCSTR FAR GetThreadName(LPTHREAD lpThread)
{
    return (LPCSTR) lpThread->Name;
}
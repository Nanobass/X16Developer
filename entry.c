#include <stdlib.h>
#include <stdio.h>

#include "cx16os.h"

void BigTask(LPVOID args)
{
    LPCSTR lpThreadName = GetThreadName(CurrentThread());
    printf("Thread Started: %s\n", lpThreadName);
    BYTE dwIndex = 0;
    do {
        printf("Thread: %s %lu\n", lpThreadName, dwIndex);
        dwIndex++;
        YieldThread();
    } while(dwIndex);
}

void SomeFunction()
{
    DWORD value;
    YieldThread();
    value += printf("HEllo\n");
    YieldThread();
    printf("%lu\n", value);
    YieldThread();
    return;
}

void SmallTask(LPVOID args)
{
    LPCSTR lpThreadName = GetThreadName(CurrentThread());
    do {
        printf("Thread: %s\n", lpThreadName);
        SomeFunction();
        YieldThread();
    } while(1);
}

extern void FAR InitializeMemory();
extern void FAR InitializeThreading();
extern int FAR Scheduler();

int main() {
    InitializeMemory();
    InitializeThreading();
    CreateThread(BigTask,   NULL, 32, "Worker-1");
    CreateThread(BigTask,   NULL, 32, "Worker-2");
    CreateThread(BigTask,   NULL, 32, "Worker-3");
    CreateThread(BigTask,   NULL, 32, "Worker-4");
    CreateThread(BigTask,   NULL, 32, "Worker-5");
    CreateThread(BigTask,   NULL, 32, "Worker-6");
    CreateThread(BigTask,   NULL, 32, "Worker-7");
    CreateThread(BigTask,   NULL, 32, "Worker-8");
    CreateThread(SmallTask, NULL, 32, " Small-1");
    CreateThread(SmallTask, NULL, 32, " Small-2");
    CreateThread(SmallTask, NULL, 32, " Small-3");
    CreateThread(SmallTask, NULL, 32, " Small-4");
    CreateThread(SmallTask, NULL, 32, " Small-5");
    CreateThread(SmallTask, NULL, 32, " Small-6");
    CreateThread(SmallTask, NULL, 32, " Small-7");
    CreateThread(SmallTask, NULL, 32, " Small-8");
    return Scheduler();
}
#include <stdlib.h>
#include <stdio.h>

#include "cx16os.h"

void App(LPVOID args)
{
    printf("Thread Started: %s\n", GetThreadName(CurrentThread()));
}

void FAR InitializeMemory();
void FAR InitializeThreading();
int FAR Scheduler();

int main() {
    InitializeMemory();
    InitializeThreading();
    CreateThread(App, NULL, 256, "Application");
    return Scheduler();
}
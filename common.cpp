#include "common.h"

void MsgBoxExit(UINT nType, LPCSTR lpCaption, LPCSTR sz, ...)
{
    char ach[512];
    va_list args;
    va_start(args, sz);
    (void)wvsprintf(ach, sz, args);
    (void)MessageBox(NULL, ach, lpCaption, nType);
    ExitProcess(0);
}
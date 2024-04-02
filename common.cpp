#include "common.h"

int MsgBoxExit(UINT nType, LPCSTR lpCaption, LPCSTR sz, ...)
{
    char ach[512];
    va_list args;
    va_start(args, sz);
    wvsprintf(ach, sz, args);
    MessageBox(NULL, ach, lpCaption, nType);
    ExitProcess(0);
}
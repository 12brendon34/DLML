#include "common.h"

//https://en.cppreference.com/w/cpp/string/byte/tolower bit lazy 
std::string str_tolower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::tolower(c); }
    );
    return s;
}

void MsgBoxExit(UINT nType, LPCSTR lpCaption, LPCSTR sz, ...)
{
    char ach[512];
    va_list args;
    va_start(args, sz);
    (void)wvsprintf(ach, sz, args);
    (void)MessageBox(NULL, ach, lpCaption, nType);
    ExitProcess(0);
}
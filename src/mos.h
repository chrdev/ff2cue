/*
* Part of ff2cue, by chrdev, 2023
* 0BSD
* 
* Multi-OS stuff
*/
#pragma once

#ifdef _WIN32

#include <wchar.h>

#define mos_main wmain(int argc, wchar_t** argv)
#define mos_fopen(f,fn,m) _wfopen_s( &f,fn,m)
#define mos_char wchar_t
#define mos_t(x) L##x
#define mos_printf wprintf_s
#define mos_fprintf fprintf_s
const char* mos_utf8(const wchar_t* wt);
#define mos_strcpy(d,sz,s) strcpy_s(d,sz,s)
#define mos_strnicmp _strnicmp

#else // not _WIN32

#define mos_main main(int argc, char** argv)
#define mos_fopen(f,fn,m) f = fopen(fn, m)
#define mos_char char
#define mos_t(x) x
#define mos_printf printf
#define mos_fprintf fprintf
#define mos_utf8(x) x
#define mos_strcpy(d,sz,s) strcpy(d,s)
#define mos_strnicmp strncasecmp

#endif // _WIN32

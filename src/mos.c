/*
* Part of ff2cue, by chrdev, 2023
* 0BSD
* 
* Multi-OS stuff
*/
#include "mos.h"

#ifdef _WIN32

#include <wchar.h>
#include <Windows.h>


const char*
mos_utf8(const wchar_t* wt) {
	static char buf[MAX_PATH];
	if (WideCharToMultiByte(CP_UTF8, 0, wt, -1, buf, sizeof(buf), NULL, NULL)) return buf;
	return NULL;
}

#endif // _WIN32

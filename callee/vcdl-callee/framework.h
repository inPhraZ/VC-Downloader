#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#define vcmalloc(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define vcfree(x) HeapFree(GetProcessHeap(), 0, (x))
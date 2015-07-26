// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <vector>
#include <array>
#include <memory>
#include <string>

#include <assert.h>

#include "wHandle.h"


const size_t MEM_COPY_BUFFER_SIZE = 4 * 1024 * 1024 - 32;
#pragma once

#include <string>

/*
	THIS FILE DEFINES API FOR THE CompressDumpMaker library
*/

#ifdef COMPRESSEDDUMPMAKER_EXPORTS
#define COMPRESSEDDUMPMAKER_API extern "C" __declspec(dllexport)
#else
#define COMPRESSEDDUMPMAKER_API __declspec(dllimport)
#endif

COMPRESSEDDUMPMAKER_API bool createDumpFile(unsigned long processId, std::wstring&& fileNameNoExt);

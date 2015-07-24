// CompressedDumpMaker.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CompressedDumpMaker.h"


// This is an example of an exported variable
COMPRESSEDDUMPMAKER_API int nCompressedDumpMaker=0;

// This is an example of an exported function.
COMPRESSEDDUMPMAKER_API int fnCompressedDumpMaker(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see CompressedDumpMaker.h for the class definition
CCompressedDumpMaker::CCompressedDumpMaker()
{
	return;
}

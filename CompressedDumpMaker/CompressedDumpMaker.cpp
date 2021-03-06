#include "stdafx.h"

#include "CompressedDumpMaker.h"
#include "DumpCreator.h"

bool createDumpFile(unsigned long processId, std::wstring& fileNameNoExt)
{
	DumpCreator dumpCreator(processId, fileNameNoExt);
	return dumpCreator.CreateDump();
}
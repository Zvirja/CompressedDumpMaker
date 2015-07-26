#include "stdafx.h"
#include "MinidumpUtils.h"


MINIDUMP_LOCATION_DESCRIPTOR* MinidumpUtils::FindStream(PMINIDUMP_HEADER pHeader, ULONG32 streamType)
{
	PMINIDUMP_DIRECTORY pFirstDirectory = (PMINIDUMP_DIRECTORY)((BYTE*)pHeader + pHeader->StreamDirectoryRva);
	for (size_t i = 0; i < pHeader->NumberOfStreams; ++i)
	{
		PMINIDUMP_DIRECTORY pCurrDir = pFirstDirectory + i;
		if (pCurrDir->StreamType == streamType)
		{
			return &pCurrDir->Location;
		}
	}

	return nullptr;
}
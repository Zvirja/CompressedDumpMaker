#pragma once
#include <DbgHelp.h>

class MinidumpUtils
{
public:
	static MINIDUMP_LOCATION_DESCRIPTOR* FindStream(PMINIDUMP_HEADER pHeader, ULONG32 streamType);
};


// LibraryRunner.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../CompressedDumpMaker/CompressedDumpMaker.h"

int _tmain(int argc, _TCHAR* argv[])
{
	createDumpFile(10992, std::wstring(L"C:\\Users\\Zvirja\\Documents\\Sources\\CompressedDumpMaker\\LibraryRunner\\TestOutput\\testFile"));



	return 0;
}


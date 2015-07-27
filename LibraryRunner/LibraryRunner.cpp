// LibraryRunner.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../CompressedDumpMaker/CompressedDumpMaker.h"
#include <iostream>
#include <chrono>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 3)
	{
		wcout << "Missed args" << endl;
		return 1;
	}

	auto pid = std::stoi(argv[1]);
	wstring outputFileName = wstring(argv[2]);
	

	auto startTime = chrono::high_resolution_clock::now();


	bool success = createDumpFile(pid, outputFileName);

	auto elapsedMsTime = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime).count();

	wcout << "Success: " << success << endl;
	wcout << "Elapsed ms time: " << elapsedMsTime << endl;

	return 0;
}


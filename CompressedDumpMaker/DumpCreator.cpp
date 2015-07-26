#include "stdafx.h"
#include "DumpCreator.h"
#include "ProcessSuspender.h"
#include "MinidumpUtils.h"

#include "MemoryListStreamPartitioner.h"


using namespace std;


DumpCreator::~DumpCreator()
{
}

bool DumpCreator::CreateDump()
{
	wHandle hTargetProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->_processId);
	if (!hTargetProc.isValid())
	{
		return false;
	}

	ProcessSuspender procSuspender(hTargetProc.get());

	if (!procSuspender.initialize())
	{
		return false;
	}

	wHandle hDumpFile = CreateFile(this->getDumpFileName().data(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (!hDumpFile.isValid() /*|| GetLastError() == ERROR_ALREADY_EXISTS*/)
	{
		return false;
	}

	if (!this->createNativeDump(hTargetProc.get(), hDumpFile.get()))
	{
		return false;
	}

	if (!this->createCompressedMemorySnapshot(hTargetProc.get(), hDumpFile.get()))
	{
		return false;
	}

	return true;
}

bool DumpCreator::createNativeDump(HANDLE hProcess, HANDLE hFile) const
{
	BOOL retCode = MiniDumpWriteDump(
		hProcess,
		this->_processId,
		hFile,
		static_cast<MINIDUMP_TYPE>(MiniDumpWithHandleData | MiniDumpWithUnloadedModules |  MiniDumpWithThreadInfo | MiniDumpWithFullMemoryInfo),
		nullptr,
		nullptr,
		nullptr);

	return retCode == TRUE;
}

bool DumpCreator::createCompressedMemorySnapshot(HANDLE hProcess, HANDLE hDumpFile) const
{
	wHandle hFileMapping = CreateFileMapping(hDumpFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
	if (!hFileMapping.isValid())
	{
		return false;
	}

	PMINIDUMP_HEADER pDumpHeader = static_cast<PMINIDUMP_HEADER>(MapViewOfFile(hFileMapping.get(), FILE_MAP_READ, 0, 0, 0));
	if (nullptr == pDumpHeader)
	{
		return false;
	}

	auto pMemoryInfoStreamLocation = MinidumpUtils::FindStream(pDumpHeader, MINIDUMP_STREAM_TYPE::MemoryInfoListStream);
	if (nullptr == pMemoryInfoStreamLocation)
	{
		return false;
	}

	PMINIDUMP_MEMORY_INFO_LIST pMemoryInfoList = (PMINIDUMP_MEMORY_INFO_LIST)((BYTE*)pDumpHeader + pMemoryInfoStreamLocation->Rva);

	MemoryListStreamPartitioner partitioner(pMemoryInfoList);

	auto compressionChunks = partitioner.getMemoryListStreamDescriptors();

	if (compressionChunks->empty())
	{
		return false;
	}

	wHandle hMemFile = CreateFile(this->getProcessMemFileName().data(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (!hMemFile.isValid() /*|| GetLastError() == ERROR_ALREADY_EXISTS*/)
	{
		return false;
	}

	if (!this->_pCompressionWorker->compressMemoryChunks(compressionChunks.get(), hMemFile.get(), hProcess))
	{
		return false;
	}


	return true;
}


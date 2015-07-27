#pragma once
#include <DbgHelp.h>
#include <concurrent_queue.h>
#include "MemoryListStreamPartitioner.h"
#include "CompressionWorkers.h"


class DumpCreator
{
public:
	DumpCreator(DWORD processId, std::wstring& fileNameNoExt) :
		_processId(processId),
		_fileNameNoExt(fileNameNoExt),
		_pMemCopyBuffer(new std::array<BYTE, MEM_COPY_BUFFER_SIZE>()),
		_pCompressionWorker(std::make_unique<LZ4CompressionWorker>())
	{
	}

	~DumpCreator();

	bool CreateDump();

private:
	DWORD _processId;
	std::wstring& _fileNameNoExt;
	std::unique_ptr <std::array<BYTE, MEM_COPY_BUFFER_SIZE> > _pMemCopyBuffer;
	std::unique_ptr<CompressionWorkerBase> _pCompressionWorker;


	std::wstring getDumpFileName() const
	{
		return this->_fileNameNoExt + L".dmp";
	}

	std::wstring getProcessMemFileName() const
	{
		return this->_fileNameNoExt + this->_pCompressionWorker->getFileExtension();
	}

	bool createNativeDump(HANDLE hProcess, HANDLE hFile) const;
	bool createCompressedMemorySnapshot(HANDLE hProcess, HANDLE hDumpFile) const;

};

